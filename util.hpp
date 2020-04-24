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

	bool is_focused( );
	std::string get_active_window_title( );
}
