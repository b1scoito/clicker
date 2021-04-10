#include "pch.hpp"
#include "clicker.hpp"

void clicker::init()
{
	/* ~~ main thread loop */
	while (true)
	{
		/* ~~ sleep for saving cpu cycles if we're not clicking to accurately click */
		if (!is_right_clicking || !is_left_clicking)
			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

		/* ~~ Changes these variables every time so if we change anything it works */
		hooks::keybinds::clicker.i_mode = config.clicker.activation_type;
		hooks::keybinds::clicker.i_key = config.clicker.clicker_key;
		vars::key::is_hotkey_enabled = hooks::keybinds::clicker.get();

		vars::window::is_focused = utils::other::focused_situation();
		vars::window::is_cursor_visible = utils::other::get_cursor_status();

		if (vars::key::is_hotkey_enabled)
		{
			if (vars::window::is_focused && vars::window::is_cursor_visible && !utils::other::application_focused())
			{
				/* ~~ Left clicker */
				if (config.clicker.left_clicker_enabled && vars::mouse::left_mouse_down)
				{
					send_click( left_mouse_button, config.clicker.left_cps, vars::mouse::left_first_click );
					is_left_clicking = true;
				}
				else
					is_left_clicking = false;

				/* ~~ Right clicker */
				if (config.clicker.right_clicker_enabled && vars::mouse::right_mouse_down)
				{
					send_click( right_mouse_button, config.clicker.right_cps, vars::mouse::right_first_click );
					is_right_clicking = true;
				}
				else
					is_right_clicking = false;
			}
		}
	}
}

void clicker::send_click( bool button, float cps, bool &is_first_click )
{
	/* ~~ Return if the cps is 0 */
	if (cps <= 0.f)
		return;

	/* ~~ If blatant is not enabled and the persistent values are enabled, apply persistent randomization, this applies to drop and spike chance too. */
	if (!config.clicker.blatant_enabled)
		cps += random;

	/* ~~ How the delay works is pretty simple, basically the delay is 1000 divided by the cps (because we're working with milliseconds)
	 * ~~ divided by 2 because the delay will be called both on input down and input up! */
	delay = (1000.f / cps) / 2.f;

	/* ~~ If blatant is not enabled apply just a little randomization */
	if (!config.clicker.blatant_enabled)
		delay += utils::floating::random( -5.f, 5.f );

	/* ~~ If it's our first click, lets wait for the delay, send the input to up and set it back to false. */
	if (is_first_click)
	{
		sleep_precisely( delay );
		send_mouse_input( input_up, button );
		is_first_click = false;
	}

	/* ~~ Sleep and input down */
	sleep_precisely( delay );
	send_mouse_input( input_down, button );

	/* ~~ Blockhit
	 * ~~ If we have blockhit enabled, the chance is higher than 0 and we hit the randomization chance, send a right input down. */
	if (config.clicker.blockhit_enabled && config.clicker.blockhit_chance > 0
		&& std::rand() % (100 / config.clicker.blockhit_chance) == 0)
	{
		blockhitted = true;
		send_mouse_input( input_down, right_mouse_button );
	}

	sleep_precisely( delay );
	send_mouse_input( input_up, button );

	/* ~~ If we blockhitted, send up input corresponding to the before down input and set variable back to false. */
	if (blockhitted)
	{
		send_mouse_input( input_up, right_mouse_button );
		blockhitted = false;
	}

	log_debug( "cps -> [ %.3f ], delay -> [ %.3fms ], button -> [ %s ]", cps, delay, translate_button( button ).c_str() );
	++vars::stats::clicks_this_session;
}

void clicker::update_thread( float delay )
{
	bool should_update { false };
	while (true)
	{
		/* ~~ If we should update. */
		if (should_update)
		{
			/* ~~ If persistent values is enabled, apply persistent values! */
			if (config.clicker.persistent_values_enabled)
				random = utils::floating::random( -config.clicker.default_persistent_randomization, config.clicker.default_persistent_randomization );

			/* ~~ If drop chance is enabled and the value is higher than 0 and the chance matches, apply it.. */
			if (config.clicker.cps_drop_chance && config.clicker.cps_drop_chance_val > 0 && std::rand() % (100 / config.clicker.cps_drop_chance_val) == 0)
				random -= utils::floating::random( (config.clicker.cps_drop_chance_value_removal - 1.f), config.clicker.cps_drop_chance_value_removal );

			/* ~~ If spike chance is enabled and the value is higher than 0 and the chance matches, apply it.. */
			if (config.clicker.cps_spike_chance && config.clicker.cps_spike_chance_val > 0 && std::rand() % (100 / config.clicker.cps_spike_chance_val) == 0)
				random += utils::floating::random( (config.clicker.cps_spike_chance_value_addition - 1.f), config.clicker.cps_spike_chance_value_addition );

			/* ~~ Set it to false se we only set it one time.. */
			should_update = false;
		}

		auto value = utils::floating::random( 1.f, delay );
		// log_debug( "Updating randomization [ %.3fms ] ", value );

		/* ~~ Wait for the next update... */
		std::this_thread::sleep_for( fl_ms( value ) );
		/* ~~ And start it!... */
		random = {};
		should_update = true;
	}
}