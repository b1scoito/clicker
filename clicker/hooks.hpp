#pragma once

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
			POINT pos { 0, 0 }; GetCursorPos( &pos );

			down ? (button ? SendMessage( GetForegroundWindow(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM( pos.x, pos.y ) ) :
				SendMessage( GetForegroundWindow(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM( pos.x, pos.y ) )) :
				(button ? SendMessage( GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM( pos.x, pos.y ) ) :
					SendMessage( GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM( pos.x, pos.y ) ));

			++var::stats::clicks_this_session;
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
		/// Init keybind loop
		/// </summary>
		void init();

		/// <summary>
		/// Keybind activation type, not very good way of doing this.
		/// </summary>
		void activation_type();
	}
}