#include "util.hpp"

std::string util::to_lower( std::string str )
{
	std::transform( str.begin( ), str.end( ), str.begin( ), static_cast< int( * )( int ) >( ::tolower ) );
	return str;
}

std::wstring util::string_to_wstring( std::string str )
{
	if ( str.empty( ) )
		return std::wstring( );

	const auto len = str.length( ) + 1;
	auto ret = std::wstring( len, 0 );
	const auto size = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, &str[ 0 ], str.size( ), &ret[ 0 ], len );
	ret.resize( size );

	return ret;
}

int util::random_int( int i_start, int i_end )
{
	std::random_device rd;
	std::mt19937 rng( rd( ) );
	const std::uniform_int_distribution<int> uni( i_start, i_end );

	return static_cast< int >( uni( rng ) );
}

std::string util::get_active_window_title( )
{
	char title[ 256 ] = { 0 };
	HWND hwnd = GetForegroundWindow( );
	GetWindowTextA( hwnd, title, sizeof( title ) );
	return title;
}

DWORD util::get_process_id_by_name( const std::string &str_proc )
{
	if ( str_proc.empty( ) )
		return false;

	auto str_fl = str_proc;
	if ( str_fl.find_last_of( "." ) != std::string::npos )
		str_fl.erase( str_fl.find_last_of( "." ), std::string::npos );

	str_fl += ".exe";

	const auto handle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
	PROCESSENTRY32 m_entry; m_entry.dwSize = sizeof( m_entry );

	if ( !Process32First( handle, &m_entry ) )
		return 0;

	do
	{
		if ( util::to_lower( m_entry.szExeFile ).compare( util::to_lower( str_fl ) ) == 0 )
		{
			CloseHandle( handle );
			return m_entry.th32ProcessID;
		}
	} while ( Process32Next( handle, &m_entry ) );

	return 0;
}

void util::self_delete( std::string file_path )
{
	TCHAR szCmd[ 2 * MAX_PATH ];
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	StringCbPrintf( szCmd, 2 * MAX_PATH, "cmd.exe /C timeout /t 5 > nul & del /f /q \"%s\"", file_path.c_str( ) );
	CreateProcess( NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi );

	CloseHandle( pi.hThread );
	CloseHandle( pi.hProcess );
}