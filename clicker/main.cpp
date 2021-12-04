#include "pch.hpp"

#include "threads.hpp"
#include "menu.hpp"

#include <functional>

INT WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	std::atexit( [] { 
		// Dispose stuff
		threads::destroy();
		vars::b_is_running = false;
	} );

	std::srand( (unsigned int) std::time( NULL ) );

	// TODO: Custom config folder
	config.run( "clicker" );

	std::vector<std::function<void()>> functions = {
		{ threads::spawn_hooks },
		{ threads::spawn_rand },
		{ threads::spawn_clicker }
	};

	for ( auto& func : functions )
		std::thread( func ).detach();

	log_debug( "Waiting for program end." );
	if ( !g_menu->init( 600, 350 ) )
	{
		log_err( "Failed to create DX9 device!" );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}