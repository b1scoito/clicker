#include "pch.hpp"
#include "hooks.hpp"

LRESULT CALLBACK hooks::mouse::m_hook_callback( int nCode, WPARAM wParam, LPARAM lParam )
{
	auto *m_hook = reinterpret_cast< MSLLHOOKSTRUCT * > ( lParam );

	if ( m_hook->flags & LLMHF_INJECTED || m_hook->flags & LLMHF_LOWER_IL_INJECTED )
	{
		m_hook->flags &= ~LLMHF_INJECTED;
		m_hook->flags &= ~LLMHF_LOWER_IL_INJECTED;
		return false;
	}

	if ( nCode == HC_ACTION && wParam != WM_MOUSEMOVE )
	{
		switch ( wParam )
		{
			case WM_LBUTTONDOWN:
				vars::b_l_first_click = true;
				vars::b_l_mouse_down = true;
				break;
			case WM_LBUTTONUP:
				vars::b_l_mouse_down = false;
				break;

			case WM_RBUTTONDOWN:
				vars::b_r_first_click = true;
				vars::b_r_mouse_down = true;
				break;
			case WM_RBUTTONUP:
				vars::b_r_mouse_down = false;
				break;
		}
	}

	return CallNextHookEx( hooks::mouse::mouse_hook, nCode, wParam, lParam );
}

LRESULT CALLBACK hooks::keyboard::k_hook_callback( int nCode, WPARAM wParam, LPARAM lParam )
{
	auto *k_hook = reinterpret_cast< KBDLLHOOKSTRUCT * >( lParam );

	if ( nCode == HC_ACTION && wParam == WM_KEYDOWN ) { /* Keybord hook (...) */ }

	return CallNextHookEx( hooks::keyboard::keyboard_hook, nCode, wParam, lParam );
}

void hooks::init_callbacks( )
{
	hooks::mouse::mouse_hook = SetWindowsHookEx
	(
		WH_MOUSE_LL,
		hooks::mouse::m_hook_callback,
		nullptr,
		NULL
	);

	/*
	hooks::keyboard::keyboard_hook = SetWindowsHookEx
	(
		WH_KEYBOARD_LL,
		hooks::keyboard::k_hook_callback,
		nullptr,
		NULL
	);
	*/

	// If this thread hangs up for processing other things, the mouse will skip in-game, because everything mouse-related passes through this hook.
	// I personally there's no fix for this.
	MSG msg {};
	while ( GetMessage( &msg, nullptr, 0, 0 ) )
	{
		if ( msg.message == WM_QUIT )
			break;

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	UnhookWindowsHookEx( hooks::mouse::mouse_hook );
	// UnhookWindowsHookEx( hooks::keyboard::keyboard_hook );
}

void hooks::keybinds::init( )
{
	while ( true )
	{
		hooks::keybinds::activation_type( );
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}
}

void hooks::keybinds::activation_type( )
{
	switch ( config.clicker.activation_type )
	{
		case 0:
			if ( config.clicker.left_enabled || config.clicker.right_enabled )
				vars::b_hotkey_enabled = true;
			break;

		case 1:
			if ( GetAsyncKeyState( config.clicker.key ) )
				vars::b_hotkey_enabled = true;
			else
				vars::b_hotkey_enabled = false;
			break;

		case 2:
			if ( GetAsyncKeyState( config.clicker.key ) )
			{
				vars::b_is_clicked = false;
				vars::b_is_down = true;
			}
			else if ( !GetAsyncKeyState( config.clicker.key ) && vars::b_is_down )
			{
				vars::b_is_clicked = true;
				vars::b_is_down = false;
			}
			else
			{
				vars::b_is_clicked = false;
				vars::b_is_down = false;
			}

			if ( vars::b_is_clicked )
				vars::b_hotkey_enabled = !vars::b_hotkey_enabled;

			break;
	}
}