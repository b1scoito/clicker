#pragma once

#include "includes.hpp"

namespace util
{
	namespace input
	{
		void left_down( );
		void left_up( );
	}

	namespace hooking
	{
		DWORD __stdcall m_hook_all( );
		LRESULT __stdcall hh_mouse_callback( int nCode, WPARAM wParam, LPARAM lParam );
	}

	std::string get_active_window_title( );
	std::string random_string( int i_size );
	int random_int( int i_start, int i_end );
}
