#include "includes.hpp"

int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd )
{

	// please use std::thread, please use hpp and cpp instead of fucking c standart, please use auto, please have BRAIN

	if ( !CreateThread( nullptr, 0, ( LPTHREAD_START_ROUTINE ) &util::hooking::m_hook_all, nullptr, 0, nullptr ) )
	{
		_log( LCRITICAL, xorstr( "failed to create hook thread, exiting." ) );
		return EXIT_FAILURE;
	}

	if ( !CreateThread( nullptr, 0, ( LPTHREAD_START_ROUTINE ) &clicker::thread, nullptr, 0, nullptr ) )
	{
		_log( LCRITICAL, xorstr( "failed to create clicker thread, exiting." ) );
		return EXIT_FAILURE;
	}

	config.run( xorstr( "clicker" ) );
	_log( LINFO, xorstr( "initialized config system" ) );

	if ( !menu::initialize( ) )
	{
		_log( LCRITICAL, xorstr( "failed to initialize imgui, exiting." ) );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}