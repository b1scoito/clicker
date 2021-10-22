#include "pch.hpp"

INT WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	std::atexit( [] { threads::hooks::unhook(); } );

	// TODO: Custom config folder
	config.run( "clicker" );

	log_debug( "Initializing threads..." );

	std::vector<std::pair<std::function<void()>, std::string>> functions =
	{
		{ threads::clicker::spawn, "clicker" },
		{ threads::randomization::spawn, "clicker randomization" },
		{ threads::hooks::spawn, "hooking" }
	};

	for ( auto& [func, name] : functions )
	{
		log_debug( "Spawning %s thread.", name.c_str() );
		std::thread( func ).detach();
	}

	log_debug( "Calling menu..." );
	if ( !g_menu.initialize( 550, 350 ) )
	{
		log_err( "Failed to create dx9 device!" );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}