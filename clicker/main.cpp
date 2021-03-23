#include "pch.hpp"

int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	// Self-destruct on exit
	std::atexit( []()
	{
		_logd( "atexit triggered, initiating self-destruct." );

		TCHAR mod_name[MAX_PATH] {};
		GetModuleFileName( NULL, mod_name, MAX_PATH );
		std::string full_path = mod_name;
		std::string file_name = full_path.substr( full_path.find_last_of( "/\\" ) + 1 );

		if (config.clicker.clear_strings_on_exit)
		{
			// Please note that only PROCESSES are accepted.
			util::other::clear_strings( {
					{ "explorer", file_name.c_str() },
					{ "explorer", full_path.c_str() },
					{ "explorer", "clicker" }
				} );
		}

		if (config.clicker.delete_file_on_exit)
			util::other::self_delete( full_path );

		if (config.clicker.delete_config_folder_on_exit)
			util::other::self_delete( config.config_path, true );

		// oh my lord
		if (config.clicker.clear_strings_on_exit
			|| config.clicker.delete_file_on_exit
			|| config.clicker.delete_config_folder_on_exit)
			Beep( 350, 200 );
	} );

	std::string disk_serial = util::other::get_disk_id();
	disk_serial.erase( std::remove_if( disk_serial.begin(), disk_serial.end(), ::isspace ), disk_serial.end() );

	config.run( disk_serial.c_str() );

	_logd( "Initializing clicker thread." );
	std::thread clicker( []() { g_clicker->init(); } );
	std::thread clicker_rand( []() { g_clicker->randomization_thread( util::numbers::random( 750.f, 1500.f ) ); } );

	_logd( "Initializing hook thread." );
	std::thread mouse( []() { hooks::init_callbacks(); } );

	_logd( "Initializing keybind thread." );
	std::thread keybinds( []() { hooks::keybinds::init(); } );

	_logd( "Initializing menu thread." );
	if (!g_menu->create( 550, 350 ))
	{
		_loge( "Failed to create menu." );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
