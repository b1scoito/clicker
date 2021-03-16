#include "pch.hpp"
#include "clicker.hpp"

void clicker::init( )
{
	while ( true )
	{
		// https://randomascii.wordpress.com/2020/10/04/windows-timer-resolution-the-great-rule-change/
		// Not sure if this will affect anything.
		timeBeginPeriod( 1 );

		// Sleep for saving CPU cycles, affects just a little on the algorithm.
		std::this_thread::sleep_for( 1ms );

		const auto get_cursor_status = [ ]( )
		{
			return config.clicker.only_in_game ? !util::other::is_cursor_visible( ) : util::other::is_cursor_visible( );
		};

		if ( vars::b_hotkey_enabled )
		{
			// check if window is active and current window is not focused
			if ( util::other::get_active_window_title( ).find( config.clicker.window_title ) != std::string::npos && !util::other::application_focused( ) )
			{
				// check if cursor is visible, in-game.
				if ( get_cursor_status( ) || !config.clicker.only_in_game )
				{
					if ( config.clicker.left_enabled && vars::b_l_mouse_down && !vars::b_r_mouse_down )
						g_clicker->click( left_mb, config.clicker.l_cps, vars::b_l_first_click );

					if ( config.clicker.right_enabled && vars::b_r_mouse_down )
						g_clicker->click( right_mb, config.clicker.r_cps, vars::b_r_first_click );
				}
			}
		}

		timeEndPeriod( 1 );
	}
}

void clicker::click( bool button, int cps, bool &is_first_click )
{
	// used to measure the time that it takes for one click precisely
#ifdef _DEBUG
	auto start = std::chrono::high_resolution_clock::now( );
#endif

	if ( cps <= 0 ) // return if our cps is 0. let's not divide it by 0.
		return;

	if ( !config.clicker.blatant )
		cps += random;

	delay = ( 1000 / cps ) / 2 - cps / 2; // delay is half because we'll be calling it two times, on mouse down and up.

	if ( !config.clicker.blatant )
		delay += util::numbers::random( 2, 5 ); // apply randomization naturally.

	if ( is_first_click ) // if it's our first click, delay and send a up input to our function.
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
		g_clicker->click_mouse( input_up, button );
		is_first_click = false;
		_log( LDEBUG, "[ clicker ] first click." );
	}

	std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
	g_clicker->click_mouse( input_down, button );

	if ( config.clicker.blockhit && config.clicker.blockhit_chance > 0 && std::rand( ) % ( 100 / config.clicker.blockhit_chance ) == 0 )
	{
		blockhitted = true;
		g_clicker->click_mouse( input_down, right_mb );
	}

	std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
	g_clicker->click_mouse( input_up, button );

	if ( blockhitted )
	{
		g_clicker->click_mouse( input_up, right_mb );
		blockhitted = false;
	}

#ifdef _DEBUG
	auto end = std::chrono::high_resolution_clock::now( );
	std::chrono::duration<double, std::milli> elapsed = end - start;
#endif

	_log( LDEBUG, "[ clicker ] cps: %d delay %d time elapsed %.3f.", cps, delay, elapsed.count( ) );
}

void clicker::randomization_thread( int delay )
{
	bool should_update = false;
	while ( true )
	{
		if ( should_update )
		{
			random = util::numbers::random( -2, 2 );

			if ( config.clicker.cps_drop_chance
				&& config.clicker.cps_drop_chance_val > 0
				&& std::rand( ) % ( 100 / config.clicker.cps_drop_chance_val ) == 0 )
				random -= 3;

			if ( config.clicker.cps_spike_chance
				&& config.clicker.cps_spike_chance_val > 0
				&& std::rand( ) % ( 100 / config.clicker.cps_spike_chance_val ) == 0 )
				random += 3;

			should_update = false;
		}

		std::this_thread::sleep_for( std::chrono::milliseconds( util::numbers::random( -delay, delay ) ) );

		should_update = true;
	}
}

void clicker::click_mouse( bool is_down, bool button )
{
	is_down ? ( button ? hooks::mouse::left_down( ) : hooks::mouse::right_down( ) ) : ( button ? hooks::mouse::left_up( ) : hooks::mouse::right_up( ) );
	++vars::i_clicks_this_session;
}