#pragma once

#include <string>
#include <memory>
#include <random>
#include <Windows.h>
#include <TlHelp32.h>
#include <strsafe.h>
#include <tchar.h>

#include "vars.hpp"

namespace util
{
	std::string to_lower( std::string str );
	std::wstring string_to_wstring( std::string str );
	int random_int( int i_start, int i_end );
	std::string get_active_window_title( );
	std::string get_serial( );
	DWORD get_process_id_by_name( const std::string &str_proc );
	void self_delete( std::string file_path );
	template<typename ...args>
	static std::string format( const std::string &format, args ...arg );
}
