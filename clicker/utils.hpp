#pragma once

#include <winternl.h>

namespace string {
	std::wstring to_unicode( std::string_view str );
}

namespace timer {
	void precise_sleep( float ms );
}

namespace rng {
	template <typename T>
	T random_range( T start, T end ) {
		std::random_device rd;
		std::mt19937_64 rng( rd() );
		std::uniform_int_distribution<T> uni( start, end );

		return (T) uni( rng );
	}

	template <typename T>
	T random_real( T start, T end ) {
		std::random_device rd;
		std::mt19937_64 rng( rd() );
		std::uniform_real_distribution<T> uni( start, end );

		return (T) uni( rng );
	}
}

namespace input {
	// TODO: Fix this logic
	enum class mouse_button_t: bool { right, left };
	enum class mouse_input_type_t: bool { up, down };
	enum class mouse_side_t: DWORD
	{
		left = MK_LBUTTON,
		right = MK_RBUTTON
	};

	enum class mouse_type_t : DWORD
	{
		left_up = WM_LBUTTONUP,
		left_down = WM_LBUTTONDOWN,

		right_up = WM_RBUTTONUP,
		right_down = WM_RBUTTONDOWN
	};

	void send_input( mouse_type_t m_type, mouse_side_t m_side );
	void click( mouse_input_type_t type, mouse_button_t button );

}

namespace focus {
	std::wstring active_window_title();

	bool is_self_focused();

	bool is_cursor_visible();

	bool window_think();

	bool cursor_think();
}