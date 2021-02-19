#pragma once

#include "../def/includes.hpp"

class mouse
{
public:
	HHOOK hk_mouse;

	void work( );
	void activation_type( );
	void keybind_work( );

	void left_down( )
	{
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput( 1, &input, sizeof( INPUT ) );
	}

	void left_up( )
	{
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput( 1, &input, sizeof( INPUT ) );
	}

	void right_down( )
	{
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		SendInput( 1, &input, sizeof( INPUT ) );
	}

	void right_up( )
	{
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		SendInput( 1, &input, sizeof( INPUT ) );
	}

	mouse( ) = default;
	~mouse( ) = default;
};

inline auto g_mouse = std::make_unique<mouse>( );