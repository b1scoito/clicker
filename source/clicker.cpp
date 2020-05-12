#include "clicker.hpp"

DWORD __stdcall clicker::work( LPVOID lParam )
{
	do
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

		if
			(
				config.clicker.enabled &&
				config.clicker.hotkey_enabled &&
				var::b_mouse_down &&
				util::window::get_active_window_title( ).find( config.clicker.window_title ) != std::string::npos
				)
		{
			if ( var::b_first_click )
			{
				std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );
				util::input::left_up( );
				var::b_first_click = false;
			}
			else
			{
				auto random_delay = util::math::random_int
				(
					1000 / ( config.clicker.min_cps + config.clicker.max_cps * ( int ) 0.2 ),
					1000 / ( config.clicker.min_cps + config.clicker.max_cps * ( int ) 0.48 )
				); // old meth logic

				if ( ( std::clock( ) - var::l_last_click_time ) > random_delay )
				{
					util::input::left_down( );
					var::l_last_click_time = std::clock( );

					std::this_thread::sleep_for( std::chrono::milliseconds( util::math::random_int( 30, 50 ) ) );
					util::input::left_up( );

					var::i_clicks_this_session++;

					_log( LDEBUG, "random_delay %d clock %d last click time %d", random_delay, std::clock( ), var::l_last_click_time );

				}
			}
		}

	} while ( true );
}