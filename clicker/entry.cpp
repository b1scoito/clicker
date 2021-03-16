#include "pch.hpp"

// WinMain definition as-is.
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	// Self-destruct on exit
	std::atexit( [ ]( )
		{
			TCHAR szFileName[ MAX_PATH ]; GetModuleFileName( NULL, szFileName, MAX_PATH );

			std::string full_path = szFileName;
			std::string file_name = full_path.substr( full_path.find_last_of( "\\/" ) + 1, full_path.find_last_of( "\\/" ) );

			// clear explorer.exe

			if ( config.clicker.clear_strings_on_exit )
			{
				auto _scanner = std::make_unique<scanner>( OpenProcess( PROCESS_ALL_ACCESS, FALSE, util::other::get_process_id_by_name( "explorer" ) ) );
				auto _ptrs = _scanner->scan_unicode( file_name.c_str( ) );
				for ( size_t _loc : _ptrs )
					_scanner->rewrite_unicode( _loc, " " );

				if ( config.clicker.clear_string_multibyte )
				{
					auto _ptrs_multibyte = _scanner->scan_multibyte( file_name.c_str( ) );
					for ( size_t _loc_m : _ptrs_multibyte )
						_scanner->rewrite_multibyte( _loc_m, " " );
				}
			}

			if ( config.clicker.delete_file_on_exit )
				util::other::self_delete( full_path, false );

			if ( config.clicker.delete_config_folder_on_exit )
				util::other::self_delete( config.config_path, true );

			// oh my lord
			if ( config.clicker.clear_strings_on_exit
				|| config.clicker.clear_string_multibyte
				|| config.clicker.delete_file_on_exit
				|| config.clicker.delete_config_folder_on_exit )
				Beep( 450, 100 );
		} );

	config.run( "clicker" );

	_log( LDEBUG, "[ clicker_thread ] Initializing clicker thread." );
	std::thread clicker( [ ]( ) { g_clicker->init( ); } );
	std::thread clicker_rand( [ ]( ) { g_clicker->randomization_thread( util::numbers::random( 500, 1500 ) ); } );

	_log( LDEBUG, "[ mouse_thread ] Initializing hook thread." );
	std::thread mouse( [ ]( ) { hooks::init_callbacks( ); } );

	_log( LDEBUG, "[ keybind_thread ] Initializing keybind thread." );
	std::thread keybinds( [ ]( ) { hooks::keybinds::init( ); } );

	_log( LDEBUG, "[ ui_create ] Initializing menu thread." );
	if ( !g_menu->create( 550, 350 ) )
	{
		_log( LERROR, "[ ui_create ] Failed to create ui, exiting." );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
