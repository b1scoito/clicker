#pragma once

#define PreciseSleep(ms) { timer::precise_sleep(ms / 1000.); }

class c_clicker
{
private:
	void send_click(input::mouse_button_t b_button, float f_cps);

	float f_delay = {};
	float f_random = {};

	bool b_should_update = {};

	bool b_is_left_clicking = {};
	bool b_is_right_clicking = {};

public:
	void thread();
	void update_cps();
	void update_variables();
};

inline auto g_clicker = std::make_unique<c_clicker>();