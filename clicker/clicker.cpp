#include "clicker.hpp"

DWORD __stdcall clicker::work( LPVOID lParam )
{
	while ( true )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

		if ( config.clicker.hotkey_enabled
			&& util::window::get_active_window_title( ).find( config.clicker.window_title ) != std::string::npos )
		{
			if ( config.clicker.left_enabled && var::b_l_mouse_down )
			{
				if ( var::b_l_first_click )
				{
					std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );
					util::input::left_up( );
					var::b_l_first_click = false;
				}
				else
				{
					auto random_delay = util::math::random_int
					(
						1000 / ( config.clicker.l_min_cps + config.clicker.l_max_cps * ( int ) 0.2 ),
						1000 / ( config.clicker.l_min_cps + config.clicker.l_max_cps * ( int ) 0.48 )
					); // old meth logic, will change to something better later ;c

					if ( ( std::clock( ) - var::l_last_click_time ) > random_delay )
					{
						util::input::left_down( );
						var::l_last_click_time = std::clock( );

						if ( config.clicker.blockhit && std::rand( ) % ( 100 / config.clicker.blockhit_chance ) == 0 )
							util::input::right_down( );

						std::this_thread::sleep_for( std::chrono::milliseconds( util::math::random_int( 30, 50 ) ) );
						util::input::left_up( );

						if ( config.clicker.blockhit )
							util::input::right_up( );

						var::i_clicks_this_session++;

						_log( LDEBUG, "l_click random_delay %d clock %d last click time %d", random_delay, std::clock( ), var::l_last_click_time );

					}
				}
			}

			// ultra weird way to make this
			if ( config.clicker.right_enabled && var::b_r_mouse_down )
			{
				if ( var::b_r_first_click )
				{
					std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );
					util::input::right_up( );
					var::b_r_first_click = false;
				}
				else
				{
					auto random_delay = util::math::random_int
					(
						1000 / ( config.clicker.r_min_cps + config.clicker.r_max_cps * ( int ) 0.2 ),
						1000 / ( config.clicker.r_min_cps + config.clicker.r_max_cps * ( int ) 0.48 )
					); // old meth logic, will change to something better later ;c

					if ( ( std::clock( ) - var::l_last_click_time ) > random_delay )
					{
						util::input::right_down( );
						var::l_last_click_time = std::clock( );

						std::this_thread::sleep_for( std::chrono::milliseconds( util::math::random_int( 30, 50 ) ) );
						util::input::right_up( );

						var::i_clicks_this_session++;

						_log( LDEBUG, "r_click random_delay %d clock %d last click time %d", random_delay, std::clock( ), var::l_last_click_time );

					}
				}
			}
		}
	}
}