#include "pch.hpp"
#include "clicker.hpp"
#include "hooks.hpp"
#include "menu.hpp"

int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	/* ~~ Self-destruct at exit */
	std::atexit( []
	{
		log_debug( "Self-destruct was triggered" );

		std::string full_path {};
		{
			TCHAR mod_name[MAX_PATH] {};
			GetModuleFileName( NULL, mod_name, MAX_PATH );
			full_path = mod_name;
		}

		std::string file_name = full_path.substr( full_path.find_last_of( "/\\" ) + 1 );

		if (config.clicker.clear_strings_on_exit)
		{
			/* ~~ Please note that only PROCESSES are accepted. */
			utils::other::clear_strings( {
					{ "explorer", file_name },
					{ "explorer", full_path },
					{ "explorer", "clicker" }
				}
			);
		}

		if (config.clicker.delete_file_on_exit)
			utils::other::self_delete( full_path );

		if (config.clicker.delete_config_folder_on_exit)
			utils::other::self_delete( config.config_path, true );

		// oh my lord
		if (config.clicker.clear_strings_on_exit || config.clicker.delete_file_on_exit
			|| config.clicker.delete_config_folder_on_exit)
			Beep( 300, 300 );
	} );

	std::string disk_serial = utils::other::get_disk_id();
	disk_serial.erase( std::remove_if( disk_serial.begin(), disk_serial.end(), ::isspace ), disk_serial.end() );

	config.run( disk_serial.c_str() );

	log_debug( "Initializing threads..." );
	std::thread clicker( [] { clicker::get().init(); } );
	std::thread clicker_update( [] { clicker::get().update_thread( utils::floating::random( 1500.f, 2500.f ) ); } );
	std::thread hooks( hooks::init_callbacks );

	log_debug( "Waiting for program end." );
	if (!menu::get().render( 550, 350 ))
	{
		log_err( "Failed to create directx9 device!" );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
