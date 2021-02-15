#include "clicker/clicker.hpp"
#include "menu/menu.hpp"

#include "def/includes.hpp"
#include "def/scanner/scanner.hpp"

int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	std::atexit( [ ]( )
		{
			TCHAR szFileName[ MAX_PATH ]; GetModuleFileName( NULL, szFileName, MAX_PATH );

			std::string full_path = szFileName;
			std::string file_name = full_path.substr( full_path.find_last_of( "\\/" ) + 1, full_path.find_last_of( "\\/" ) );

			// clear explorer.exe
			{
				auto _scanner = std::make_unique<scanner>( OpenProcess( PROCESS_ALL_ACCESS, FALSE, util::get_process_id_by_name( "explorer" ) ) );

				auto _ptrs = _scanner->scan_unicode( file_name.c_str( ) );

				for ( size_t _loc : _ptrs )
					_scanner->rewrite_unicode( _loc, " " );

				if ( config.clicker.clear_unicode_multibyte )
				{
					auto _ptrs_multibyte = _scanner->scan_mltbyte( file_name.c_str( ) );

					for ( size_t _loc_m : _ptrs_multibyte )
						_scanner->rewrite_mltbyte( _loc_m, " " );
				}

				if ( config.clicker.delete_file_on_exit )
				{
					util::self_delete( full_path );
				}
			}

			// clear other processes {...}

		} );

	_log( LDEBUG, "Initializing." );

	config.run( util::get_serial( ).c_str( ) );

	std::thread thread_mouse( [ ]( ) { g_mouse->work( ); } );

	std::thread thread_clicker( [ ]( ) { g_clicker->work( ); } );

	if ( !g_menu->create( 550, 350 ) )
	{
		_log( LERROR, "Failed to create ui, exiting." );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
