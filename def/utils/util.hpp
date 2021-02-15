#pragma once

#include <string>
#include <memory>
#include <random>
#include <Windows.h>

#include "vars.hpp"

namespace util
{
	namespace hooking
	{
		inline HHOOK hook_mouse;

		DWORD __stdcall work( );
		LRESULT __stdcall mouse( int nCode, WPARAM wParam, LPARAM lParam );
	}

	namespace input
	{
		void left_down( );
		void left_up( );
		void right_down( );
		void right_up( );
	}

	std::string to_lower( std::string str );
	int random_int( int i_start, int i_end );

	std::string get_active_window_title( );
	std::string get_serial( );
}
