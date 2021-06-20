#pragma once

namespace vars
{
	namespace key
	{
		inline auto clicker_enabled = keybind( true, config.clicker.i_key_type, config.clicker.i_clicker_key );
		inline auto left_clicker_down = keybind( false, keybind_state_t::hold, VK_LBUTTON );
		inline auto right_clicker_down = keybind( false, keybind_state_t::hold, VK_RBUTTON );
		inline auto hide_window = keybind( false, keybind_state_t::toggle, config.clicker.i_hide_window_key );
		inline auto inventory_opened = false;
	}

	namespace stats
	{
		inline auto f_average_cps { 0.f };
		inline auto i_clicks_this_session { 0 };
	}
}