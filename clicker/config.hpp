#pragma once

#include "archivex.hpp"

#include <shlobj.h>
#include <fstream>
#include <filesystem>

class c_config
{
private:
	std::filesystem::path path;
	std::vector<std::string> configs;
public:
	void run( std::string name );
	void load( size_t id );
	void save( size_t id ) const;
	void add( std::string name );
	void remove( size_t id );
	void rename( size_t item, std::string new_name );
	void reset();

	constexpr auto& get_configs() { return configs; };
	constexpr auto& get_path() { return path; };

	struct
	{
		int i_clicker_key { 0 };
		int i_key_type { 0 };
		int i_version_type { 0 };

		bool b_enable_left_clicker { false };
		bool b_enable_right_clicker { false };

		float f_left_cps { 11.5f };
		float f_right_cps { 11.5f };

		bool b_enable_blockhit { false };
		int i_blockhit_chance { 5 };

		bool b_enable_blatant { false };

		int i_hide_window_key { 0 };

		bool b_enable_advanced_options { false };

		bool b_enable_persistence { false };
		float f_persistence_value { 2.5f };

		float f_persistence_update_rate { 2000.f };
		float f_default_timer_randomization { 5.f };

		int i_cps_spike_chance { 5 };
		float f_cps_spike_add { 2.5f };

		int i_cps_drop_chance { 15 };
		float f_cps_drop_remove { 3.5f };

		bool b_enable_cps_spikes { false };
		bool b_enable_cps_drops { false };

		bool b_only_in_game { false };
		bool b_work_in_inventory { false };

		float f_color_accent[4] { 0.41f, 0.41f, 0.86f, 1.00f };
		float f_color_accent_hovered[4] { 0.53f, 0.53f, 0.91f, 1.00f };
		float f_color_accent_active[4] { 0.40f, 0.39f, 0.90f, 1.00f };
		float f_color_accent_text[4] { 0.94f, 0.94f, 0.94f, 1.00f };

		std::string str_window_title;

	} clicker;
};

inline auto config = c_config();