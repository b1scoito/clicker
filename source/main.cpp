#include "includes.hpp"

int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd )
{
	if ( !LI_FN( CreateThread ).safe_cached( )( nullptr, 0, ( LPTHREAD_START_ROUTINE ) &util::hooking::m_hook_all, nullptr, 0, nullptr ) )
	{
		_log( LCRITICAL, xorstr( "failed to create hook thread, exiting." ) );
		return EXIT_FAILURE;
	}

	if ( !LI_FN( CreateThread ).safe_cached( )( nullptr, 0, ( LPTHREAD_START_ROUTINE ) &clicker::work, nullptr, 0, nullptr ) )
	{
		_log( LCRITICAL, xorstr( "failed to create clicker thread, exiting." ) );
		return EXIT_FAILURE;
	}

	config.run( xorstr( "pop_rdi_ret" ) ); // this will be statically analyzed, if you want an actual config system do a web-based one (https://github.com/whoshuu/cpr)

	ui::initialize( );

	return EXIT_SUCCESS;
}