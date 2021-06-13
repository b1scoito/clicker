#include "pch.hpp"
#include "clicker.hpp"
#include "menu.hpp"

// ~ main entrypoint
INT WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	std::atexit( [] {} );

	config.run( "clicker" );

	log_debug( "initializing threads..." );

	if ( !_beginthreadex( nullptr, 0,
		reinterpret_cast<unsigned( __stdcall* )( void* )>( thread::click::init ), nullptr, 0, nullptr ) )
	{
		log_err( "Failed to initialize clicker thread!" );
		return EXIT_FAILURE;
	}

	if ( !_beginthreadex( nullptr, 0,
		reinterpret_cast<unsigned( __stdcall* )( void* )>( thread::click::randomization ), nullptr, 0, nullptr ) )
	{
		log_err( "Failed to initialize clicker randomization thread!" );
		return EXIT_FAILURE;
	}

	if ( !_beginthreadex( nullptr, 0,
		reinterpret_cast<unsigned( __stdcall* )( void* )>( thread::hooking::spawn ), nullptr, 0, nullptr ) )
	{
		log_err( "Failed to initialize hooking!" );
		return EXIT_FAILURE;
	}

	log_debug( "waiting for program end." );
	if ( !menu.initialize( 550, 350 ) )
	{
		log_err( "Failed to create DirectX9 device!" );
	}

	return EXIT_SUCCESS;
}