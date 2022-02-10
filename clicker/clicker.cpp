#include "pch.hpp"
#include "clicker.hpp"

void c_clicker::initialize()
{
	while ( vars::b_is_running )
	{
		if ( !this->is_right_clicking || !this->is_left_clicking )
			std::this_thread::sleep_for( 1ms );

		vars::key::clicker_enabled.i_mode = config.clicker.i_key_type; 
		vars::key::clicker_enabled.i_key = config.clicker.i_clicker_key;

		if ( vars::key::clicker_enabled.get() ) {
			if ( focus::window_think() && focus::cursor_think() ) {
				// left
				this->is_left_clicking = ( config.clicker.b_enable_left_clicker && vars::key::is_left_down.get());
				if ( this->is_left_clicking )
					this->send_click(input::mouse_button_t::left, config.clicker.f_left_cps);

				// right
				this->is_right_clicking = ( config.clicker.b_enable_right_clicker && vars::key::is_right_down.get());
				if ( this->is_right_clicking )
					this->send_click(input::mouse_button_t::right, config.clicker.f_right_cps);
			}
		}
	}
}

void c_clicker::send_click(input::mouse_button_t b_button, float f_cps )
{
	const auto start = std::chrono::high_resolution_clock::now();

	if ( f_cps <= 0 )
		return;

	if ( !config.clicker.b_enable_blatant )
		f_cps += this->random;

	this->delay = ( 1000 / f_cps ) / 2;

	if ( !config.clicker.b_enable_blatant )
		this->delay += rng::random_real<float>( -config.clicker.f_default_timer_randomization, 
			config.clicker.f_default_timer_randomization );

	PreciseSleep( this->delay );
	input::click( input::mouse_input_type_t::down, b_button );

	if ( config.clicker.b_enable_blockhit && 
		config.clicker.i_blockhit_chance > 0 && 
		std::rand() % ( 100 / config.clicker.i_blockhit_chance ) == 0 )
	{
		this->blockhitted = true;
		input::click( input::mouse_input_type_t::down, input::mouse_button_t::right );
	}

	PreciseSleep( this->delay );
	input::click( input::mouse_input_type_t::up, b_button );

	if (this->blockhitted)
	{
		input::click(input::mouse_input_type_t::up, input::mouse_button_t::right );
		blockhitted = false;
	}

	const auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed { end - start };

	vars::stats::f_average_cps = (float) ( 1000.f / elapsed.count() );

	++vars::stats::i_clicks_this_session;

	log_debug( "[%d, %s]: CPS: [ %.3f ] Delay: [ %.3fms ] Time elapsed: [ %.3fms ] Avg CPS: [ %.3f ]",
		vars::stats::i_clicks_this_session,
		config.clicker.i_send_input_method ? "PostMessage" : "SendMessage",
		f_cps,
		( this->delay * 2 ),
		elapsed.count(),
		vars::stats::f_average_cps
	);
}

void c_clicker::update()
{
	while ( vars::b_is_running )
	{
		if ( !this->is_right_clicking || !this->is_left_clicking )
			std::this_thread::sleep_for( 1ms );

		auto rate = rng::random_real<float>( 0, config.clicker.f_persistence_update_rate );

		if ( this->should_update )
		{
			// Persistence
			if ( config.clicker.b_enable_persistence )
				random = rng::random_real<float>( -config.clicker.f_persistence_value,
					config.clicker.f_persistence_value );

			// CPS Drops
			if ( config.clicker.b_enable_cps_drops &&
				config.clicker.i_cps_drop_chance > 0 &&
				( std::rand() % ( 100 / config.clicker.i_cps_drop_chance ) == 0 ) )
			{
				random -= config.clicker.f_cps_drop_remove;
			}

			// CPS Spikes
			if ( config.clicker.b_enable_cps_spikes &&
				config.clicker.i_cps_spike_chance > 0 &&
				( std::rand() % ( 100 / config.clicker.i_cps_spike_chance ) == 0 ) )
			{
				random += config.clicker.f_cps_spike_add;
			}

			this->should_update = false;
		}

		PreciseSleep( rate );

		this->random = {};
		this->should_update = true;
	}
}