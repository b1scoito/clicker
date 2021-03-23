#pragma once

namespace var
{
	namespace mouse
	{
		inline bool left_mouse_down { false };
		inline bool left_first_click { false };

		inline bool right_mouse_down { false };
		inline bool right_first_click { false };
	}

	namespace key
	{
		inline bool is_clicked { false };
		inline bool is_down { false };

		inline bool is_inventory_opened { false };

		inline bool is_hotkey_enabled { false };
	}

	namespace stats
	{
		inline int clicks_this_session { 0 };
	}
}