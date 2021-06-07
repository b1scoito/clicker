#include "pch.hpp"
#include "clicker.hpp"
#include "menu.hpp"

auto init_self_destruct() -> void;

// ~ main entrypoint
//
int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	std::atexit( init_self_destruct );

	config.run( "clicker" );

	log_debug( "initializing threads..." );

	if ( !_beginthreadex( nullptr, 0,
		reinterpret_cast<unsigned( __stdcall* )( void* )>( initialize_clicker_thread ), nullptr, 0, nullptr ) )
	{
		log_err( "Failed to initialize clicker thread!" );
		return EXIT_FAILURE;
	}

	if ( !_beginthreadex( nullptr, 0,
		reinterpret_cast<unsigned( __stdcall* )( void* )>( initialize_clicker_randomization_thread ), nullptr, 0, nullptr ) )
	{
		log_err( "Failed to initialize clicker randomization thread!" );
		return EXIT_FAILURE;
	}

	if ( !_beginthreadex( nullptr, 0,
		reinterpret_cast<unsigned( __stdcall* )( void* )>( hooking::spawn ), nullptr, 0, nullptr ) )
	{
		log_err( "Failed to initialize hooking!" );
		return EXIT_FAILURE;
	}

	log_debug( "waiting for program end." );
	if ( !menu.init_rendering( 550, 350 ) )
	{
		log_err( "Failed to create directx9 device!" );
	}


	return EXIT_SUCCESS;
}

auto init_self_destruct() -> void
{
	/// ...
}