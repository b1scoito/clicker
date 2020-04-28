#pragma once

namespace var
{
	inline bool b_mouse_down { false };
	inline bool b_first_click { false };

	inline int i_current_cps { 0 };
	inline int i_clicks_this_session { 0 };
	inline long l_last_click_time;

	inline const char *items[] = { "Minecraft", "Badlion",  "Lunar", "Custom" };

	namespace fonts
	{

	}
}