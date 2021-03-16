#pragma once

#include <bitset>

namespace hooks
{
	/// <summary>
	/// Initialize hook callbacks
	/// </summary>
	void init_callbacks( );

	namespace mouse
	{
		/// <summary>
		/// Definition of our mouse hook struct
		/// </summary>
		inline HHOOK mouse_hook {};

		/// <summary>
		/// Mouse hook callback
		/// </summary>
		LRESULT CALLBACK m_hook_callback( int nCode, WPARAM wParam, LPARAM lParam );

		/// <summary>
		/// Click left down
		/// </summary>
		inline void left_down( )
		{
			INPUT input {};
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput( 1, &input, sizeof( INPUT ) );
		}

		/// <summary>
		/// Click left up
		/// </summary>
		inline void left_up( )
		{
			INPUT input {};
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput( 1, &input, sizeof( INPUT ) );
		}

		/// <summary>
		/// Click right down
		/// </summary>
		inline void right_down( )
		{
			INPUT input {};
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
			SendInput( 1, &input, sizeof( INPUT ) );
		}

		/// <summary>
		/// Click right up
		/// </summary>
		inline void right_up( )
		{
			INPUT input {};
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
			SendInput( 1, &input, sizeof( INPUT ) );
		}
	}

	namespace keyboard
	{
		/// <summary>
		/// Definition of our keyboard hook struct
		/// </summary>
		inline HHOOK keyboard_hook {};

		/// <summary>
		/// Keyboard hook callback function
		/// </summary>
		/// <param name="nCode"></param>
		/// <param name="wParam"></param>
		/// <param name="lParam"></param>
		/// <returns></returns>
		LRESULT CALLBACK k_hook_callback( int nCode, WPARAM wParam, LPARAM lParam );
	}

	namespace keybinds
	{
		/// <summary>
		/// Init keybind loop
		/// </summary>
		void init( );

		/// <summary>
		/// Keybind activation type, not very good way of doing this.
		/// </summary>
		void activation_type( );
	}
}