#pragma once

#include "../def/includes.hpp"
#include "../mouse/mouse.hpp"

#define sleep(ms) (std::this_thread::sleep_for(std::chrono::milliseconds(ms)));

class clicker
{
public:
	void work( );
	void click( bool button, int cps, bool &is_first_click );
	void click_mouse( bool is_down, bool button );

	~clicker( ) = default;
	clicker( ) = default;

private:
	int delay = 0; // delay for the sleep
	int random = 0; // random cps

	bool blockhitted = false;

	bool right_mb = false;
	bool left_mb = true;

	bool input_down = true;
	bool input_up = false;
};

inline auto g_clicker = std::make_unique<clicker>( );