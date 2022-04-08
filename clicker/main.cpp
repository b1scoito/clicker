#include "pch.hpp"

#include "clicker.hpp"
#include "menu.hpp"

INT WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	std::atexit( [] { 
		// Dispose
		vars::b_is_running = false;
	} );

	// Set random seed
	std::srand( (std::uint32_t) std::time( NULL ) );

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	// Initialize threads
	std::thread(&c_clicker::thread, g_clicker.get()).detach();
	std::thread(&c_clicker::update_cps, g_clicker.get()).detach();
	std::thread(&c_clicker::update_variables, g_clicker.get()).detach();

	// TODO: Custom config folder
	config.run( "clicker" );

	// Setup menu
	log_debug( "Waiting for program end." );
	if ( !g_menu->setup() )
	{
		log_err( "Failed to setup menu!" );
		g_menu->destroy();

		return EXIT_FAILURE;
	}
	
	// Menu renderer loop
	g_menu->render();

	return EXIT_SUCCESS;
}