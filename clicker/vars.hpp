#pragma once

namespace vars
{
	namespace key
	{
		inline auto clicker_enabled { keybind( true, config.clicker.i_key_type, config.clicker.i_clicker_key ) };
		inline auto hide_window { keybind( false, keybind_state_t::toggle, config.clicker.i_hide_window_key ) };

		inline auto is_left_down { false };
		inline auto is_right_down { false };

		inline auto l_is_first_click { false };
		inline auto r_is_first_click { false };

		inline auto b_inventory_opened { false };
	}

	namespace stats
	{
		inline auto f_average_cps { 0.f };
		inline auto i_clicks_this_session { 0 };
	}
}