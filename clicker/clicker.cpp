#include "pch.hpp"
#include "clicker.hpp"

void c_clicker::init()
{
	while ( true )
	{
		if ( !this->m_is_right_clicking || !this->m_is_left_clicking )
			std::this_thread::sleep_for( 1ms );

		var::key::clicker_enabled.i_mode = config.clicker.i_key_type;
		var::key::clicker_enabled.i_key = config.clicker.i_clicker_key;

		static bool first_click = true;
		if ( var::key::clicker_enabled.get() )
		{
			if ( util::extra::is_window_focused() && util::extra::is_cursor_visible() && !( util::extra::is_application_focused() ) )
			{
				this->m_is_left_clicking = ( config.clicker.b_enable_left_clicker && var::key::left_clicker_down.get() && !var::key::right_clicker_down.get() );
				if ( this->m_is_left_clicking )
					this->send_click( button_t::left, config.clicker.f_left_cps, first_click );

				this->m_is_right_clicking = ( config.clicker.b_enable_right_clicker && var::key::right_clicker_down.get() );
				if ( this->m_is_right_clicking )
					this->send_click( button_t::right, config.clicker.f_right_cps, first_click );
			}
		}
	}
}

void c_clicker::send_click( button_t b_button, float f_cps, bool& b_is_first_click )
{
	const auto start = std::chrono::high_resolution_clock::now();

	// ~~ return if the cps is 0
	if ( f_cps <= 0 )
		return;

	/* ~~ If blatant is not enabled and the persistent values are enabled, apply persistent randomization, this applies to drop and spike chance too. */
	if ( !config.clicker.b_enable_blatant )
		f_cps += this->m_random;

	/* ~~ How the delay works is pretty simple, basically the delay is 1000 divided by the cps (because we're working with milliseconds)
	 * ~~ divided by 2 because the delay will be called both on input down and input up! */
	this->m_delay = ( 1000 / f_cps ) / 2;

	// ~~ if blatant is not enabled apply given randomization values
	if ( !config.clicker.b_enable_blatant )
		this->m_delay += util::random::number( -config.clicker.f_default_timer_randomization,
			config.clicker.f_default_timer_randomization );

	// ~~ if it's our first click, lets wait for the delay, send the input to up and set it back to false.
	if ( b_is_first_click )
	{
		sleep( this->m_delay );
		this->send_mouse_input( input_type_t::up, b_button );
		b_is_first_click = false;
	}

	// ~~ sleep and input down
	sleep( this->m_delay );
	this->send_mouse_input( input_type_t::down, b_button );

	/* ~~ Blockhit
	 * ~~ If we have blockhit enabled, the chance is higher than 0 and we hit the randomization chance, send a right input down. */
	if ( config.clicker.b_enable_blockhit && config.clicker.i_blockhit_chance > 0
		&& std::rand() % ( 100 / config.clicker.i_blockhit_chance ) == 0 )
	{
		this->m_blockhitted = true;
		this->send_mouse_input( input_type_t::down, button_t::right );
	}

	sleep( this->m_delay );
	this->send_mouse_input( input_type_t::up, b_button );

	// ~~ if we blockhitted, send up input corresponding to the before down input and set variable back to false.
	if ( this->m_blockhitted )
	{
		send_mouse_input( input_type_t::up, button_t::right );
		m_blockhitted = false;
	}

	++var::stats::i_clicks_this_session;

	const auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed { end - start };

	log_debug( "cps: [ %.3f ] delay: [ %.3fms ] time elapsed: [ %.3fms ]", f_cps, ( this->m_delay * 2 ), elapsed.count() );
}

void c_clicker::update_thread()
{
	while ( true )
	{
		// ~~ if we should update.
		if ( this->m_should_update )
		{
			// ~~ if persistent values is enabled, apply persistent values!
			if ( config.clicker.b_enable_persistence )
				m_random = util::random::number( -config.clicker.f_persistence_value, config.clicker.f_persistence_value );

			// ~~ if drop chance is enabled and the value is higher than 0 and the chance matches, apply it..
			if ( config.clicker.b_enable_cps_drops &&
				config.clicker.i_cps_drop_chance > 0 && ( std::rand() % ( 100 / config.clicker.i_cps_drop_chance ) == 0 ) )
				m_random -= config.clicker.f_cps_drop_remove;

			// ~~ if spike chance is enabled and the value is higher than 0 and the chance matches, apply it..
			if ( config.clicker.b_enable_cps_spikes &&
				config.clicker.i_cps_spike_chance > 0 && ( std::rand() % ( 100 / config.clicker.i_cps_spike_chance ) == 0 ) )
				m_random += config.clicker.f_cps_spike_add;

			// ~~ set it to false so we only set it one time..
			this->m_should_update = false;
		}

		m_persistent_value = util::random::number( config.clicker.f_persistence_update_rate / 2, config.clicker.f_persistence_update_rate );

		// ~~ wait for the next update...
		std::this_thread::sleep_for( floating_ms( config.clicker.f_persistence_update_rate ) );

		// ~~ and start it!...
		this->m_random = {};
		this->m_should_update = true;
	}
}