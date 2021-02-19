#pragma once

#include <string>
#include <memory>
#include <random>
#include <Windows.h>
#include <TlHelp32.h>
#include <strsafe.h>
#include <tchar.h>

#include "../vars.hpp"

namespace util
{
	std::string to_lower( std::string str );
	std::wstring string_to_wstring( std::string str );
	int random_int( int start, int end );
	std::string get_active_window_title( );
	DWORD get_process_id_by_name( const std::string &p_name );
	void self_delete( std::string file_path );
}
