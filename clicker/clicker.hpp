#pragma once

// for timeBeginPeriod
#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

using floating_ms = std::chrono::duration<float, std::chrono::milliseconds::period>;

enum class button_t: bool
{
	right, left
};

enum class input_type_t: bool
{
	up, down
};

#define sleep(ms) { timeBeginPeriod(1); g_clicker.precise_timer_sleep( (double)( ms / 1000 ) ); timeEndPeriod(1); }

class c_clicker
{
private:
	void send_click( button_t b_button, float f_cps, bool& b_is_first_click );

	void precise_timer_sleep( double seconds )
	{
		while ( seconds > 5e-3 )
		{
			auto start = std::chrono::high_resolution_clock::now();
			std::this_thread::sleep_for( 1ms );
			auto end = std::chrono::high_resolution_clock::now();

			auto observed = ( end - start ).count() / 1e9;
			seconds -= observed;
		}

		/* ~~ spin lock */
		const auto start = std::chrono::high_resolution_clock::now();
		while ( ( std::chrono::high_resolution_clock::now() - start ).count() / 1e9 < seconds );
	}

	void send_mouse_input( input_type_t i_type, button_t b_button )
	{
		(bool) ( i_type ) ? (bool) ( b_button ) ? input::left_down() : input::right_down() : (bool) ( b_button ) ? input::left_up() : input::right_up();
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