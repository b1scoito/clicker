#include "clicker/clicker.hpp"
#include "menu/ui.hpp"

#include "def/includes.hpp"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	std::atexit( [ ]( )
		{
			// self-destruct
		} );

	_log( LDEBUG, "Initializing." );

	config.run( util::get_serial( ).c_str( ) );

	if ( !CreateThread( nullptr, 0, ( LPTHREAD_START_ROUTINE ) &util::hooking::work, nullptr, 0, nullptr ) )
	{
		_log( LERROR, "Failed to create hook thread, exiting." );
		return EXIT_FAILURE;
	}

	if ( !CreateThread( nullptr, 0, ( LPTHREAD_START_ROUTINE ) &clicker::work, nullptr, 0, nullptr ) )
	{
		_log( LERROR, "Failed to create clicker thread, exiting." );
		return EXIT_FAILURE;
	}

	if ( !g_menu->create( 550, 350 ) )
	{
		_log( LERROR, "Failed to create ui, exiting." );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
