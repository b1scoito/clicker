#include "pch.hpp"

INT WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	std::atexit( [] { threads::hooking::unhook(); } );

	config.run( "clicker" );

	log_debug( "initializing threads..." );

	std::vector<std::pair<void*, std::string>> functions =
	{
		{ threads::clicker::init, "clicker" },
		{ threads::clicker::randomization, "clicker randomization" },
		{ threads::hooking::spawn, "hooking" }
	};

	for ( auto& [func, name] : functions )
	{
		log_debug( "spawning %s thread...", name.c_str() );

		if ( !_beginthreadex( nullptr, 0, reinterpret_cast<unsigned( __stdcall* )( void* )>( func ), nullptr, 0, nullptr ) )
		{
			log_err( "failed to spawn %s thread!", name.c_str() );
			return EXIT_FAILURE;
		}
	}

	log_debug( "calling menu..." );
	if ( !g_menu.initialize( 550, 350 ) )
	{
		log_err( "failed to create DirectX9 device!" );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}