#include "pch.hpp"
#include "clicker.hpp"

// TODO: Fix clicker
void c_clicker::init()
{
	while ( true )
	{
		if ( !this->m_is_right_clicking || !this->m_is_left_clicking )
			std::this_thread::sleep_for( 1ms );

		vars::key::clicker_enabled.i_mode = config.clicker.i_key_type;
		vars::key::clicker_enabled.i_key = config.clicker.i_clicker_key;

		static auto first_click = true;
		if ( vars::key::clicker_enabled.get() )
		{
			if ( focus::window_think() && focus::cursor_think() && !focus::is_self_focused() )
			{
				// left
				//
				this->m_is_left_clicking = ( config.clicker.b_enable_left_clicker && vars::key::left_mouse.get() && !vars::key::right_mouse.b_is_down);

				if ( this->m_is_left_clicking )
					this->send_click(input::mouse_button_t::left, config.clicker.f_left_cps, first_click );

				// right
				//
				this->m_is_right_clicking = ( config.clicker.b_enable_right_clicker && vars::key::right_mouse.get());

				if ( this->m_is_right_clicking )
					this->send_click(input::mouse_button_t::right, config.clicker.f_right_cps, first_click );
			}
		}
	}
}

void c_clicker::send_click(input::mouse_button_t b_button, float f_cps, bool& b_is_first_click )
{
	const auto start = std::chrono::high_resolution_clock::now();

	if ( f_cps <= 0 )
		return;

	if ( !config.clicker.b_enable_blatant )
		f_cps += this->m_random;

	this->m_delay = ( 1000 / f_cps ) / 2;

	if ( !config.clicker.b_enable_blatant )
		this->m_delay += rng::random_real<float>( -config.clicker.f_default_timer_randomization, config.clicker.f_default_timer_randomization );
	
	// TODO: Fudeu!
	if ( b_is_first_click )
	{
		sleep( this->m_delay );
		input::click( input::mouse_input_type_t::up, b_button );
		b_is_first_click = false;
	}

	sleep( this->m_delay );
	input::click( input::mouse_input_type_t::down, b_button );

	if ( config.clicker.b_enable_blockhit && config.clicker.i_blockhit_chance > 0 && std::rand() % ( 100 / config.clicker.i_blockhit_chance ) == 0 )
	{
		this->m_blockhitted = true;
		input::click( input::mouse_input_type_t::down, input::mouse_button_t::right );
	}

	sleep( this->m_delay );
	input::click( input::mouse_input_type_t::up, b_button );

	if (this->m_blockhitted)
	{
		input::click(input::mouse_input_type_t::up, b_button);
		m_blockhitted = false;
	}

	const auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed { end - start };

	vars::stats::f_average_cps = (float) ( 1000.f / elapsed.count() );

	++vars::stats::i_clicks_this_session;

	log_debug( "cps: [ %.3f ] delay: [ %.3fms ] time elapsed: [ %.3fms ]", f_cps, ( this->m_delay * 2 ), elapsed.count() );
}

void c_clicker::update_thread()
{
	while ( true )
	{
		std::this_thread::sleep_for( 1ms );

		if ( this->m_is_right_clicking || this->m_is_left_clicking )
		{
			auto rate = rng::random_real<float>( 0, config.clicker.f_persistence_update_rate );

			if ( this->m_should_update )
			{
				log_debug( "Update %.3fms", rate );

				// ~ persistence
				if ( config.clicker.b_enable_persistence )
					m_random = rng::random_real<float>( -config.clicker.f_persistence_value, config.clicker.f_persistence_value );

				// ~ cps drops
				if ( config.clicker.b_enable_cps_drops && config.clicker.i_cps_drop_chance > 0 && ( std::rand() % ( 100 / config.clicker.i_cps_drop_chance ) == 0 ) )
					m_random -= config.clicker.f_cps_drop_remove;

				// ~ cps spikes
				if ( config.clicker.b_enable_cps_spikes && config.clicker.i_cps_spike_chance > 0 && ( std::rand() % ( 100 / config.clicker.i_cps_spike_chance ) == 0 ) )
					m_random += config.clicker.f_cps_spike_add;

				this->m_should_update = false;
			}

			sleep( rate );

			this->m_random = {};
			this->m_should_update = true;
		}
	}
}