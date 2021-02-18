#pragma once

#include "../def/includes.hpp"
#include "../mouse/mouse.hpp"

#define sleep(ms) (std::this_thread::sleep_for(std::chrono::milliseconds(ms)));

class clicker
{
public:
	void work( );
	void click( bool b_button, int i_cps, bool *b_first_click );
	void click_mouse( bool down, bool button );

	~clicker( ) = default;
	clicker( ) = default;

private:
	int delay = 0; // delay for the sleep
	int random = 0; // random cps
	bool if_blockhitted = false;

	bool RBUTTON = false;
	bool LBUTTON = true;

	bool INPUT_UP = false;
	bool INPUT_DOWN = true;
};

inline auto g_clicker = std::make_unique<clicker>( );