#include "pch.hpp"
#include "clicker.hpp"

void c_clicker::thread()
{
	SetThreadPriority(std::this_thread::get_id, THREAD_PRIORITY_TIME_CRITICAL);

	// Loop while running
	while ( vars::b_is_running )
	{
		if ( !this->b_is_right_clicking || !this->b_is_left_clicking )
			std::this_thread::sleep_for( 1ms );

		if ( vars::key::clicker_enabled.get() ) 
		{
			if ( focus::window_think() && focus::cursor_think() ) 
			{
				// Left button
				this->b_is_left_clicking = ( config.clicker.b_enable_left_clicker && vars::key::is_left_down.get() && !vars::key::is_right_down.get());
				if (this->b_is_left_clicking )
					send_click(input::mouse_button_t::left, config.clicker.f_left_cps);

				// Right button
				this->b_is_right_clicking = ( config.clicker.b_enable_right_clicker && vars::key::is_right_down.get());
				if (this->b_is_right_clicking )
					send_click(input::mouse_button_t::right, config.clicker.f_right_cps);
			}
		}
	}
}

void c_clicker::send_click(input::mouse_button_t b_button, float f_cps )
{
	const auto start = std::chrono::high_resolution_clock::now();

	if (f_cps <= 0 )
		return;

	if ( !config.clicker.b_enable_blatant )
		f_cps += this->f_random;

	this->f_delay = (1000 / f_cps) / 2;

	if ( !config.clicker.b_enable_blatant )
		this->f_delay += rng::random_real<float>(1.f, config.clicker.f_default_timer_randomization );

	// Down delay
	PreciseSleep((double)this->f_delay);
	input::click( input::mouse_input_type_t::down, b_button );

	// Up delay
	PreciseSleep((double)this->f_delay);
	input::click( input::mouse_input_type_t::up, b_button );

	const auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed { end - start };
	vars::stats::f_average_cps = (float) ( 1000.f / elapsed.count() );

	++vars::stats::i_clicks_this_session;

	log_debug( "[%d]: CPS: %.3f | delay: %.3fms | time elapsed: %.3fms | avg. CPS: %.3f",
		vars::stats::i_clicks_this_session, f_cps, (this->f_delay * 2), elapsed.count(), vars::stats::f_average_cps);
}

void c_clicker::update_cps()
{
	while ( vars::b_is_running )
	{
		if ( !this->b_is_right_clicking || !this->b_is_left_clicking )
			std::this_thread::sleep_for( 1ms );

		const auto rate = rng::random_real<float>( 1500.f, config.clicker.f_persistence_update_rate );

		if ( b_should_update )
		{
			log_debug("Update %.3fms", rate);

			// Persistence
			if (config.clicker.b_enable_persistence)
				this->f_random = rng::random_real<float>( 1.f, config.clicker.f_persistence_value );

			// CPS Drops
			if (config.clicker.b_enable_cps_drops && 
				config.clicker.i_cps_drop_chance > 0 && (std::rand() % (100 / config.clicker.i_cps_drop_chance) == 0))
			{
				this->f_random -= config.clicker.f_cps_drop_remove;
			}

			// CPS Spikes
			if (config.clicker.b_enable_cps_spikes &&
				config.clicker.i_cps_spike_chance > 0 && (std::rand() % (100 / config.clicker.i_cps_spike_chance) == 0))
			{
				this->f_random += config.clicker.f_cps_spike_add;
			}

			this->b_should_update = false;
		}

		PreciseSleep((double)rate);

		this->f_random = {};
		this->b_should_update = true;
	}
}

void c_clicker::update_variables()
{
	while (vars::b_is_running)
	{
		if (!this->b_is_right_clicking || !this->b_is_left_clicking)
			std::this_thread::sleep_for(1ms);

		vars::key::clicker_enabled.i_mode = config.clicker.i_key_type;
		vars::key::clicker_enabled.i_key = config.clicker.i_clicker_key;
		
		if (vars::key::clicker_enabled.get()) {
			if (focus::window_think())
			{
				vars::key::b_shift_pressed = GetAsyncKeyState(VK_SHIFT);

				if (GetAsyncKeyState(69) || GetAsyncKeyState(VK_RSHIFT))
				{
					vars::key::b_inventory_opened = !vars::key::b_inventory_opened;
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
				}

				if (GetAsyncKeyState(VK_ESCAPE))
				{
					vars::key::b_inventory_opened = false;
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
				}
			}
		}
	}
}