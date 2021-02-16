#pragma once

#include "archivex.hpp"
#include "../../def/includes.hpp"

#include <ShlObj.h>
#include <fstream>
#include <filesystem>

class c_config final
{
public:
	std::string config_path;
	void run( const char * );
	void load( size_t );
	void save( size_t ) const;
	void add( const char * );
	void remove( size_t );
	void rename( size_t, const char * );
	void reset( );
	constexpr auto &get_configs( ) { return configs; }

	struct
	{
		bool left_enabled { false };
		bool right_enabled { false };

		bool hotkey_enabled { false };
		bool blockhit { false };

		bool delete_file_on_exit { false };
		bool clear_string_on_exit { false };
		bool clear_string_multibyte { false };

		int key { 0 };
		int activation_type { 0 };
		int version_type { 0 };

		int l_min_cps { 0 };
		int l_max_cps { 0 };

		int r_min_cps { 0 };
		int r_max_cps { 0 };

		int blockhit_chance { 0 };

		std::string window_title;
	}
	clicker;

private:
	std::filesystem::path path;
	std::vector<std::string> configs;
};

extern c_config config;