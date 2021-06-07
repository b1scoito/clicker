#pragma once

#include <timeapi.h>
#pragma comment(lib, "Winmm.lib")

using floating_ms = std::chrono::duration<float, std::chrono::milliseconds::period>;

enum buttons: bool
{
	RIGHT = false,
	LEFT = true
};

enum input_types: bool
{
	UP = false,
	DOWN = true
};

#define sleep(ms) { timeBeginPeriod(1); clicker.precise_timer_sleep( static_cast<double>( ms / 1000.f ) ); timeEndPeriod(1); }

class c_clicker
{
private:
	auto send_click( bool button, float cps, bool& is_first_click ) -> void;

	auto precise_timer_sleep( double seconds ) -> void
	{
		while ( seconds > 5e-3 )
		{
			auto start = std::chrono::high_resolution_clock::now();
			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
			auto end = std::chrono::high_resolution_clock::now();

			auto observed = ( end - start ).count() / 1e9;
			seconds -= observed;
		}

		/* ~~ spin lock */
		auto start = std::chrono::high_resolution_clock::now();
		while ( ( std::chrono::high_resolution_clock::now() - start ).count() / 1e9 < seconds );
	}

	auto translate_button( bool button ) -> std::string
	{
		switch ( button )
		{
			case 0:
				return "right mouse button";
			case 1:
				return "left mouse button";
		}

		return {};
	}

	void send_mouse_input( bool down, bool button )
	{
		POINT pos;
		GetCursorPos( &pos );

		down ? ( button ? PostMessage( GetForegroundWindow(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM( pos.x, pos.y ) ) :
			PostMessage( GetForegroundWindow(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM( pos.x, pos.y ) ) ) :
			( button ? PostMessage( GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM( pos.x, pos.y ) ) :
				PostMessage( GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM( pos.x, pos.y ) ) );
	}

	float m_delay = 0.f;
	float m_random = 0.f;

	bool m_blockhitted = false;

	bool m_is_left_clicking = false;
	bool m_is_right_clicking = false;
public:
	void init();
	void update_thread( float delay );

	~c_clicker() = default;
	c_clicker() = default;
};

inline auto clicker = c_clicker();

inline auto initialize_clicker_thread() -> void
{
	clicker.init();
}

inline auto initialize_clicker_randomization_thread() -> void
{
	clicker.update_thread( util::random::number( 1500, 3500 ) );
}

namespace hooking
{
	inline HHOOK h_hook;

	static LRESULT CALLBACK keyboard_callback( int nCode, WPARAM wParam, LPARAM lParam )
	{
		static auto* k_hook = reinterpret_cast<KBDLLHOOKSTRUCT*>( lParam );

		if ( wParam == WM_KEYDOWN && nCode == HC_ACTION && ( wParam >= WM_KEYFIRST ) && ( wParam <= WM_KEYLAST ) )
		{
			if ( util::extra::is_window_focused() )
			{
				if ( k_hook->vkCode == 69 )
					var::key::inventory_opened = !var::key::inventory_opened;

				if ( k_hook->vkCode == VK_ESCAPE )
					var::key::inventory_opened = false;
			}
		}

		return CallNextHookEx( h_hook, nCode, wParam, lParam );
	}

	inline auto spawn() -> void
	{
		h_hook = SetWindowsHookEx(
			WH_KEYBOARD_LL,
			keyboard_callback,
			nullptr,
			NULL
		);

		MSG msg;
		while ( GetMessage( &msg, nullptr, 0, 0 ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		UnhookWindowsHookEx( h_hook );
	}
}