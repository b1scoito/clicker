#include "pch.hpp"
#include "clicker.hpp"

void clicker::init()
{
	while (true)
	{
		// Sleep for saving CPU cycles, affects just a little on the algorithm.
		std::this_thread::sleep_for( 1ms );

		const auto get_cursor_status = []()
		{
			return config.clicker.only_in_game ? !util::other::is_cursor_visible() : util::other::is_cursor_visible();
		};

		if (var::b_hotkey_enabled)
		{
			// check if window is active and current window is not focused
			if (util::other::get_active_window_title().find( config.clicker.window_title ) != std::string::npos
				&& !util::other::application_focused())
			{
				// check if cursor is visible, in-game.
				if (get_cursor_status() || config.clicker.work_on_inventory && (var::b_inventory_opened || !config.clicker.only_in_game))
				{
					if (config.clicker.left_enabled && var::b_l_mouse_down && !var::b_r_mouse_down)
						click( left_mb, config.clicker.l_cps, var::b_l_first_click );

					if (config.clicker.right_enabled && var::b_r_mouse_down)
						click( right_mb, config.clicker.r_cps, var::b_r_first_click );
				}
			}
		}
	}
}

void clicker::click( bool button, float cps, bool &is_first_click )
{
	// return if our cps is 0. let's not divide it by 0.
	if (cps <= 0.f)
		return;

	if (!config.clicker.blatant)
		// applying persistent randomization before delay calculation
		cps += random;

	// pretty weird, what might be happening here?! I had to subtract the cps divided by 2 otherwise it wouldn't go as accurate.
	// delay is half because we'll be calling it two times, on mouse down and up.
	delay = (1000.f / cps) / 2.f - cps / 2.f;

	if (!config.clicker.blatant)
		// apply randomization by default.
		delay += util::numbers::random( -5.0f, 5.0f );

	// if it's our first click, delay and send a up input to our function.
	if (is_first_click)
	{
		std::this_thread::sleep_for( floating_point_ms( delay ) );
		hooks::mouse::send_mouse_input( input_up, button );
		is_first_click = false;
		_logd( "first click." );
	}

	// sleep
	std::this_thread::sleep_for( floating_point_ms( delay ) );

	// click
	hooks::mouse::send_mouse_input( input_down, button );

	if (config.clicker.blockhit && config.clicker.blockhit_chance > 0 && std::rand() % (100 / config.clicker.blockhit_chance) == 0)
	{
		blockhitted = true;
		hooks::mouse::send_mouse_input( input_down, right_mb );
	}

	// sleep
	std::this_thread::sleep_for( floating_point_ms( delay ) );

	// click
	hooks::mouse::send_mouse_input( input_up, button );

	// if we blockhitted before, let's set input to up and set variable back to false
	if (blockhitted)
	{
		hooks::mouse::send_mouse_input( input_up, right_mb );
		blockhitted = false;
	}

	_logd( "cps: %.1f delay: %.1f.", cps, delay );
}

void clicker::randomization_thread( float delay )
{
	bool should_update = false;
	while (true)
	{
		if (should_update)
		{
			// persistent cps values
			random = util::numbers::random( -1.5f, 1.5f );

			// cps drop chance
			if (config.clicker.cps_drop_chance && config.clicker.cps_drop_chance_val > 0
				&& std::rand() % (100 / config.clicker.cps_drop_chance_val) == 0)
				// add up 2.5 cps
				random -= 2.5f;

			// cps spike chance
			if (config.clicker.cps_spike_chance && config.clicker.cps_spike_chance_val > 0
				&& std::rand() % (100 / config.clicker.cps_spike_chance_val) == 0)
				// add up 2.5 cps
				random += 2.5f;

			should_update = false;
		}

		// random delay between before defined values on entrypoint, this adds to more randomization of the persistent values.
		std::this_thread::sleep_for( floating_point_ms( util::numbers::random( -delay, delay ) ) );
		should_update = true;
	}
}