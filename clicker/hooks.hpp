#pragma once

#include "keybind.hpp"

namespace hooks
{
	/// <summary>
	/// Initialize hook callbacks
	/// </summary>
	void init_callbacks();

	namespace mouse
	{
		/// <summary>
		/// Definition of our mouse hook struct
		/// </summary>
		inline HHOOK h_hook {};

		/// <summary>
		/// Mouse hook callback, this hook is replaced by the Windows
		/// so everything that happens on the mouse get's passed through this function when the hook is used.
		/// </summary>
		LRESULT CALLBACK m_hook_callback( int nCode, WPARAM wParam, LPARAM lParam );

		/// <summary>
		/// Clicks a mouse button
		/// </summary>
		inline void send_mouse_input( bool down, bool button )
		{
			POINT pos {}; GetCursorPos( &pos );

			down ? (button ? SendMessage( GetForegroundWindow(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM( pos.x, pos.y ) ) :
				SendMessage( GetForegroundWindow(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM( pos.x, pos.y ) )) :
				(button ? SendMessage( GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM( pos.x, pos.y ) ) :
					SendMessage( GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM( pos.x, pos.y ) ));
		}
	}

	namespace keyboard
	{
		/// <summary>
		/// Definition of our keyboard hook struct
		/// </summary>
		inline HHOOK h_hook {};

		/// <summary>
		/// Keyboard hook callback function, everything that keyboard does gets passed here.
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
		/// Clicker keybind class spawn
		/// </summary>
		inline keybind clicker = keybind( true, config.clicker.activation_type, config.clicker.clicker_key );

		/// <summary>
		/// Hide window class spawn
		/// </summary>
		inline keybind hide_window = keybind( false, 2, config.clicker.hide_window_key );
	}
}