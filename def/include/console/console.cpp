#include "console.hpp"

#include <filesystem>

console::console( )
{
#ifdef _DEBUG
	initialize( );
#endif
}

console::~console( )
{
#ifdef _DEBUG
	close( );
#endif
}

void console::initialize( )
{
#ifdef _DEBUG
	FILE *conin, *conout;

	LI_FN( AllocConsole ).cached( )( );
	LI_FN( AttachConsole ).cached( )( LI_FN( GetCurrentProcessId ).cached( )( ) );

	const auto str_title = util::string::random( util::math::random_int( 10, 15 ) );
	LI_FN( SetConsoleTitleA ).cached( )( str_title.c_str( ) );

	freopen_s( &conin, xorstr( "conin$" ), "r", stdin );
	freopen_s( &conout, xorstr( "conout$" ), "w", stdout );
	freopen_s( &conout, xorstr( "conout$" ), "w", stderr );
#endif
}

void console::close( )
{
#ifdef _DEBUG
	const auto handle = LI_FN( FindWindowA ).cached( )( xorstr( "ConsoleWindowClass" ), nullptr );
	LI_FN( ShowWindow ).cached( )( handle, SW_HIDE );
	LI_FN( FreeConsole ).cached( )( );
#endif
}