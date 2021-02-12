#pragma once

#include "../includes.hpp"

namespace var
{
	inline bool b_l_mouse_down { false };
	inline bool b_l_first_click { false };

	inline bool b_r_first_click { false };
	inline bool b_r_mouse_down { false };

	inline bool b_is_clicked { false };
	inline bool b_is_down { false };

	inline int i_clicks_this_session { 0 };

	inline long l_last_click_time;
}