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
		DWORD __stdcall work( );
		LRESULT __stdcall mouse( int nCode, WPARAM wParam, LPARAM lParam );
	}

	namespace string
	{
		template<typename ... args>
		static std::string format( const std::string &format, args ... arg )
		{
			const size_t size = std::snprintf( nullptr, 0, format.c_str( ), arg ... ) + 1;
			std::unique_ptr<char[]> buf( new char[ size ] );
			std::snprintf( buf.get( ), size, format.c_str( ), arg ... );
			return std::string( buf.get( ), buf.get( ) + size - 1 );
		}

	}

	std::string get_active_window_title( );
	std::string random_string( int i_size );
	int random_int( int i_start, int i_end );
}
