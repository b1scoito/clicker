#include "pch.hpp"

#include "clicker.hpp"
#include "menu.hpp"

INT WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	std::atexit( [] { 
		// Dispose stuff
		vars::b_is_running = false;
	} );

	std::srand( (std::uint32_t) std::time( NULL ) );

	std::thread(&c_clicker::initialize, c_clicker()).detach();
	std::thread(&c_clicker::update, c_clicker()).detach();

	// TODO: Custom config folder
	config.run( "clicker" );

	log_debug( "Waiting for program end." );
	if ( !g_menu->init( 600, 350 ) )
	{
		log_err( "Failed to create DX9 device!" );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}