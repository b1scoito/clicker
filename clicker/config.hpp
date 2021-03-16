#pragma once

#include "archivex.hpp"

#include <ShlObj.h>
#include <fstream>
#include <filesystem>

class c_config final
{
public:
	/// <summary>
	/// Global definition of the configuration path
	/// </summary>
	std::string config_path;

	/// <summary>
	/// Run config system
	/// </summary>
	/// <param name=""></param>
	void run( const char * );

	/// <summary>
	/// Loads a config
	/// </summary>
	/// <param name=""></param>
	void load( size_t );

	/// <summary>
	/// Saves a config
	/// </summary>
	/// <param name=""></param>
	void save( size_t ) const;

	/// <summary>
	/// Creates a configuration file
	/// </summary>
	/// <param name=""></param>
	void add( const char * );

	/// <summary>
	/// Removes a configuration file
	/// </summary>
	/// <param name=""></param>
	void remove( size_t );

	/// <summary>
	/// Renames a configuration file
	/// </summary>
	/// <param name=""></param>
	/// <param name=""></param>
	void rename( size_t, const char * );

	/// <summary>
	/// Resets a config
	/// </summary>
	void reset( );

	/// <summary>
	/// Returns config list
	/// </summary>
	/// <returns></returns>
	constexpr auto &get_configs( ) { return configs; }

	/// <summary>
	/// Definition of the config variables
	/// </summary>
	struct
	{
		bool left_enabled { false };
		bool right_enabled { false };

		bool blockhit { false };
		bool blatant { false };

		bool delete_file_on_exit { false };
		bool delete_config_folder_on_exit { false };
		bool clear_strings_on_exit { false };
		bool clear_string_multibyte { false };

		int key { 0 };
		int activation_type { 0 };
		int version_type { 0 };

		int l_cps { 1 };
		int r_cps { 1 };

		int blockhit_chance { 5 };
		int cps_spike_chance_val { 5 };
		int cps_drop_chance_val { 5 };

		bool cps_spike_chance { false };
		bool cps_drop_chance { false };

		bool only_in_game { false };

		std::string window_title;
	} clicker;

private:
	/// <summary>
	/// Configuration path
	/// </summary>
	std::filesystem::path path;

	/// <summary>
	/// List of configs
	/// </summary>
	std::vector<std::string> configs;
};

extern c_config config;