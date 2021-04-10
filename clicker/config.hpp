#pragma once

#include "archivex.hpp"
#include "imgui/imgui.h"

#include <ShlObj.h>
#include <fstream>
#include <filesystem>

class c_config final
{
private:
	/// <summary>
	/// Configuration path
	/// </summary>
	std::filesystem::path path;

	/// <summary>
	/// List of configs
	/// </summary>
	std::vector<std::string> configs;
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
	void reset();

	/// <summary>
	/// Returns config list
	/// </summary>
	/// <returns></returns>
	constexpr auto &get_configs() { return configs; }

	/// <summary>
	/// Definition of the config variables
	/// </summary>
	struct
	{
		bool left_clicker_enabled { false };
		bool right_clicker_enabled { false };

		bool blockhit_enabled { false };
		bool blatant_enabled { false };

		bool delete_file_on_exit { false };
		bool delete_config_folder_on_exit { false };
		bool clear_strings_on_exit { false };

		int clicker_key { 0 };
		int hide_window_key { 0 };
		int activation_type { 0 };
		int version_type { 0 };

		float left_cps { 1.f };
		float right_cps { 1.f };

		int blockhit_chance { 5 };

		bool persistent_values_enabled { true };
		float default_persistent_randomization { 2.f };

		int cps_spike_chance_val { 15 };
		float cps_spike_chance_value_addition { 3.5f };

		int cps_drop_chance_val { 10 };
		float cps_drop_chance_value_removal { 3.5f };

		bool cps_spike_chance { false };
		bool cps_drop_chance { false };

		bool only_in_game { false };
		bool work_in_inventory { false };

		float color_accent[4] { 0.41f, 0.41f, 0.86f, 1.00f };
		float color_accent_hovered[4] { 0.53f, 0.53f, 0.91f, 1.00f };
		float color_accent_active[4] { 0.40f, 0.39f, 0.90f, 1.00f };
		float color_accent_text[4] { 0.94f, 0.94f, 0.94f, 1.00f };

		std::string window_title;
	}
	clicker;
};

extern c_config config;