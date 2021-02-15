#include "clicker.hpp"

DWORD __stdcall clicker::work( LPVOID lParam )
{
	while ( true )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

		if ( config.clicker.hotkey_enabled && util::get_active_window_title( ).find( config.clicker.window_title ) != std::string::npos )
		{
			if ( config.clicker.left_enabled && vars::b_l_mouse_down )
			{
				if ( vars::b_l_first_click )
				{
					std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );
					util::input::left_up( );
					vars::b_l_first_click = false;
				}
				else
				{
					auto random_delay = util::random_int
					(
						1000 / ( config.clicker.l_min_cps + config.clicker.l_max_cps * ( int ) 0.2 ),
						1000 / ( config.clicker.l_min_cps + config.clicker.l_max_cps * ( int ) 0.48 )
					); // old meth logic, will change to something better later ;c

					if ( ( std::clock( ) - vars::l_last_click_time ) > random_delay )
					{
						util::input::left_down( );
						vars::l_last_click_time = std::clock( );

						if ( config.clicker.blockhit && config.clicker.blockhit_chance > 0 && std::rand( ) % ( 100 / config.clicker.blockhit_chance ) == 0 )
							util::input::right_down( );

						std::this_thread::sleep_for( std::chrono::milliseconds( util::random_int( 30, 50 ) ) );
						util::input::left_up( );

						if ( config.clicker.blockhit )
							util::input::right_up( );

						vars::i_clicks_this_session++;

						_log( LDEBUG, "l_click random_delay %d clock %d last click time %d", random_delay, std::clock( ), vars::l_last_click_time );
					}
				}
			}

			if ( config.clicker.right_enabled && vars::b_r_mouse_down )
			{
				if ( vars::b_r_first_click )
				{
					std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );
					util::input::right_up( );
					vars::b_r_first_click = false;
				}
				else
				{
					auto random_delay = util::random_int
					(
						1000 / ( config.clicker.r_min_cps + config.clicker.r_max_cps * ( int ) 0.2 ),
						1000 / ( config.clicker.r_min_cps + config.clicker.r_max_cps * ( int ) 0.48 )
					);

					if ( ( std::clock( ) - vars::l_last_click_time ) > random_delay )
					{
						util::input::right_down( );
						vars::l_last_click_time = std::clock( );

						std::this_thread::sleep_for( std::chrono::milliseconds( util::random_int( 30, 50 ) ) );
						util::input::right_up( );

						vars::i_clicks_this_session++;

						_log( LDEBUG, "r_click random_delay %d clock %d last click time %d", random_delay, std::clock( ), vars::l_last_click_time );
					}
				}
			}
		}
	}
}