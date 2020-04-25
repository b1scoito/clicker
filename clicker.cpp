#pragma once

#include "clicker.hpp"

DWORD WINAPI clicker::thread( LPVOID lParam )
{
	do
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

		if ( config.clicker.enabled && vars::b_mouse_down && util::get_active_window_title( ).find( config.clicker.window_name ) != std::string::npos )
		{
			if ( vars::b_first_click )
			{
				std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );
				util::input::left_up( );
				vars::b_first_click = false;
			}
			else
			{
				auto random_delay = math::random_int( ( int ) config.clicker.min_cps * 10, ( int ) config.clicker.max_cps * 10 );

				vars::d_current_cps = random_delay / 10;

				if ( ( std::clock( ) - vars::l_last_click_time ) > random_delay )
				{
					util::input::left_down( );
					vars::l_last_click_time = std::clock( );

					std::this_thread::sleep_for( std::chrono::milliseconds( math::random_int( 30, 50 ) ) );

					util::input::left_up( );
				}
			}
		}

	} while ( true );
}