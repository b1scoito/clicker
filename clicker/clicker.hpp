#pragma once

#define PreciseSleep(ms) { timer::precise_sleep(ms); }

class c_clicker
{
private:
	void send_click( input::mouse_button_t b_button, float f_cps );

	float delay = 0.f;
	float random = 0.f;

	bool blockhitted = false;
	bool should_update = false;

	bool is_left_clicking = false;
	bool is_right_clicking = false;

public:
	void initialize();
	void update();

	~c_clicker() = default;
	c_clicker() = default;
};

inline auto g_clicker = std::make_unique<c_clicker>();