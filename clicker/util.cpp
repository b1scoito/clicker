#include "pch.hpp"
#include "util.hpp"

std::string util::string::to_lower( std::string str )
{
	std::transform( str.begin( ), str.end( ), str.begin( ), static_cast< int( * )( int ) >( ::tolower ) );
	return str;
}

std::wstring util::string::string_to_wstring( std::string str )
{
	if ( str.empty( ) )
		return std::wstring( );

	const auto len = str.length( ) + 1;
	auto ret = std::wstring( len, 0 );
	const auto size = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, &str[ 0 ], str.size( ), &ret[ 0 ], len );
	ret.resize( size );

	return ret;
}

template<typename ... args>
static std::string util::string::format( const std::string &format, args ... arg )
{
	const size_t size = std::snprintf( nullptr, 0, format.c_str( ), arg ... ) + 1;
	std::unique_ptr<char[]> buf( new char[ size ] );
	std::snprintf( buf.get( ), size, format.c_str( ), arg ... );
	return std::string( buf.get( ), buf.get( ) + size - 1 );
}

int util::numbers::random( int start, int end )
{
	std::random_device rd;
	std::mt19937 rng( rd( ) );
	const std::uniform_int_distribution<int> uni( start, end );

	return static_cast< int >( uni( rng ) );
}

std::string util::other::get_active_window_title( )
{
	char title[ 256 ] = { 0 };
	HWND hwnd = GetForegroundWindow( );
	GetWindowText( hwnd, title, sizeof( title ) );
	return title;
}

DWORD util::other::get_process_id_by_name( const std::string &p_name )
{
	if ( p_name.empty( ) )
		return false;

	auto str_fl = p_name;
	if ( str_fl.find_last_of( "." ) != std::string::npos )
		str_fl.erase( str_fl.find_last_of( "." ), std::string::npos );

	str_fl += ".exe";

	const auto handle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
	PROCESSENTRY32 m_entry {}; m_entry.dwSize = sizeof( m_entry );

	if ( !Process32First( handle, &m_entry ) )
		return 0;

	while ( Process32Next( handle, &m_entry ) )
	{
		if ( util::string::to_lower( m_entry.szExeFile ).compare( util::string::to_lower( str_fl ) ) == 0 )
		{
			CloseHandle( handle );
			return m_entry.th32ProcessID;
		}
	}

	return 0;
}

void util::other::self_delete( std::string path, bool is_folder )
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	std::string cmd_line = util::string::format( "cmd.exe /C ping 1.1.1.1 > nul & del /f /q %s", path.c_str( ) ).c_str( );
	if ( is_folder )
		cmd_line = util::string::format( "cmd.exe /C ping 1.1.1.1 > nul & rmdir %s", path.c_str( ) ).c_str( );

	CreateProcess( NULL, const_cast< LPSTR >( cmd_line.c_str( ) ), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi );

	CloseHandle( pi.hThread );
	CloseHandle( pi.hProcess );
}

bool util::other::application_focused( )
{
	HWND h_wnd = GetForegroundWindow( );
	DWORD dw_current_process_id = GetCurrentProcessId( );
	DWORD dw_thread_process_id {};

	if ( !h_wnd )
		return false;

	GetWindowThreadProcessId( h_wnd, &dw_thread_process_id );
	return ( dw_current_process_id == dw_thread_process_id );
}

bool util::other::is_cursor_visible( )
{
	CURSORINFO mouse_info { sizeof( CURSORINFO ) };

	if ( GetCursorInfo( &mouse_info ) )
	{
		HCURSOR handle { mouse_info.hCursor };

		if ( int( handle ) > 50000 && int( handle ) < 100000 )
			return true;
	}

	return false;
}