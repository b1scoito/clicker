#include "includes.hpp"

int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd )
{
	_log( LDEBUG, xorstr( "Initializing" ) ); // added to construct class when program start

	config.run( xorstr( "w_w" ) ); // this will be statically analyzed, if you want an actual config system do a web-based one (https://github.com/whoshuu/cpr)

	if ( !LI_FN( CreateThread ).cached( )( nullptr, 0, ( LPTHREAD_START_ROUTINE ) &util::hooking::work, nullptr, 0, nullptr ) ) // creating mouse hook loop
	{
		_log( LCRITICAL, xorstr( "Failed to create hook thread, exiting." ) );
		return EXIT_FAILURE;
	}

	if ( !LI_FN( CreateThread ).cached( )( nullptr, 0, ( LPTHREAD_START_ROUTINE ) &clicker::work, nullptr, 0, nullptr ) ) // creating clicker loop
	{
		_log( LCRITICAL, xorstr( "Failed to create clicker thread, exiting." ) );
		return EXIT_FAILURE;
	}

	if ( !ui::create( ) ) // creating window and d3d9 render loop
	{
		_log( LCRITICAL, xorstr( "Failed to create ui, exiting." ) );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
