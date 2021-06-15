#pragma once

// for timeBeginPeriod
#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

using floating_ms = std::chrono::duration<float, std::chrono::milliseconds::period>;

enum button_t: bool
{
	right, left
};

enum input_type_t: bool
{
	up, down
};

#define sleep(ms) { timeBeginPeriod(1); g_clicker.precise_timer_sleep( static_cast<double>( ms / 1000 ) ); timeEndPeriod(1); }

class c_clicker
{
private:
	void send_click( button_t b_button, float f_cps, bool& b_is_first_click );

	void precise_timer_sleep( double f_seconds )
	{
		while ( f_seconds > 5e-3 )
		{
			auto start = std::chrono::high_resolution_clock::now();
			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
			auto end = std::chrono::high_resolution_clock::now();

			auto observed = ( end - start ).count() / 1e9;
			f_seconds -= observed;
		}

		/* ~~ spin lock */
		auto start = std::chrono::high_resolution_clock::now();
		while ( ( std::chrono::high_resolution_clock::now() - start ).count() / 1e9 < f_seconds );
	}

	void send_mouse_input( input_type_t i_type, button_t b_button )
	{
		POINT pos; GetCursorPos( &pos );

		i_type ? ( b_button ? PostMessage( GetForegroundWindow(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM( pos.x, pos.y ) ) :
			PostMessage( GetForegroundWindow(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM( pos.x, pos.y ) ) ) :
			( b_button ? PostMessage( GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM( pos.x, pos.y ) ) :
				PostMessage( GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM( pos.x, pos.y ) ) );
	}

	float m_delay { 0.f };
	float m_random { 0.f };

	bool m_blockhitted { false };

	bool m_should_update { false };

	bool m_is_left_clicking { false };
	bool m_is_right_clicking { false };

public:
	void init();
	void update_thread();

	~c_clicker() = default;
	c_clicker() = default;
};

inline auto g_clicker = c_clicker();