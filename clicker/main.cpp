#include "pch.hpp"

INT WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	std::atexit( [] { threads::hooks::unhook(); } );
	std::srand( std::time( nullptr ) );

	// TODO: Custom config folder
	config.run( "clicker" );

	std::vector<std::pair<std::function<void()>, std::string>> functions = {
		{ threads::clicker::spawn, "clicker" },
		{ threads::randomization::spawn, "clicker randomization" },
		{ threads::hooks::spawn, "hooking" }
	};

	for ( auto& [func, name] : functions )
		std::thread( func ).detach();

	log_debug( "Main loop started..." );
	if ( !g_menu.initialize( 550, 350 ) )
	{
		log_err( "Failed to create DX9 device!" );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}