#pragma once

#define sleep(ms) { timer::precise_sleep(ms); }

class c_clicker
{
private:
	void send_click( input::mouse_button_t b_button, float f_cps, bool& b_is_first_click );

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

inline auto g_clicker = std::make_unique<c_clicker>();