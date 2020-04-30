#include "includes.hpp"

int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd )
{
	_log( LDEBUG, xorstr( "initializing program." ) ); // added to construct class when program start

	if ( !LI_FN( CreateThread ).safe_cached( )( nullptr, 0, ( LPTHREAD_START_ROUTINE ) &util::hooking::work, nullptr, 0, nullptr ) ) // creating mouse hook thread
	{
		_log( LCRITICAL, xorstr( "failed to create hook thread, exiting." ) );
		return EXIT_FAILURE;
	}

	if ( !LI_FN( CreateThread ).safe_cached( )( nullptr, 0, ( LPTHREAD_START_ROUTINE ) &clicker::work, nullptr, 0, nullptr ) ) // creating clicker thread
	{
		_log( LCRITICAL, xorstr( "failed to create clicker thread, exiting." ) );
		return EXIT_FAILURE;
	}

	config.run( xorstr( "pop_rdi_ret" ) ); // this will be statically analyzed, if you want an actual config system do a web-based one (https://github.com/whoshuu/cpr)

	if ( !ui::create( ) ) // creating window and d3d9 render
	{
		_log( LCRITICAL, xorstr( "failed to create ui, exiting." ) );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}