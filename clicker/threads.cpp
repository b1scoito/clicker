#include "pch.hpp"

#include "threads.hpp"

DWORD WINAPI threads::spawn_hooks()
{
	oLowLevelKeyboardProc = SetWindowsHookEx( WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0 );
	if ( !oLowLevelKeyboardProc )
	{
		// Failed to setup WH_KEYBOARD_LL
		return EXIT_SUCCESS;
	}

	oLowLevelMouseProc = SetWindowsHookEx( WH_MOUSE_LL, LowLevelMouseProc, nullptr, 0 );
	if ( !oLowLevelMouseProc )
	{
		// Failed to setup WH_MOUSE_LL
		return EXIT_SUCCESS;
	}

	MSG msg {};
	ZeroMemory( &msg, sizeof( msg ) );

	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			continue;
		}

		WaitMessage();
	}

	return EXIT_SUCCESS;
}

LRESULT CALLBACK threads::LowLevelMouseProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if ( nCode == HC_ACTION )
	{
		auto mi = *( (MSLLHOOKSTRUCT*) lParam );

		switch ( wParam )
		{
			case WM_LBUTTONDOWN:
				vars::key::l_is_first_click = true;
				vars::key::is_left_down = true;
				break;
			case WM_LBUTTONUP: 
				vars::key::is_left_down = false;
				break;
			case WM_MOUSEMOVE: break;
			case WM_MOUSEWHEEL: break;
			case WM_MOUSEHWHEEL: break; 
			case WM_RBUTTONDOWN:
				vars::key::r_is_first_click = true;
				vars::key::is_right_down = true;
				break; 
			case WM_RBUTTONUP: 
				vars::key::is_right_down = false;
				break;
		}
	}

	return CallNextHookEx( oLowLevelMouseProc, nCode, wParam, lParam );
}

LRESULT CALLBACK threads::LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if ( nCode == HC_ACTION )
	{
		auto mi = *( (KBDLLHOOKSTRUCT*) lParam );

		switch ( wParam )
		{
			case WM_KEYDOWN:
				if ( focus::window_think() )
				{
					if ( mi.vkCode == 69 /* E */ )
						vars::key::b_inventory_opened = !vars::key::b_inventory_opened;

					if ( mi.vkCode == VK_ESCAPE )
						vars::key::b_inventory_opened = false;
				}
				break;
			case WM_SYSKEYDOWN: break;
			case WM_KEYUP: break;
			case WM_SYSKEYUP: break;
		}
	}
	 
	return CallNextHookEx( oLowLevelKeyboardProc, nCode, wParam, lParam );
}

void threads::destroy()
{
	UnhookWindowsHookEx( oLowLevelMouseProc );
	UnhookWindowsHookEx( oLowLevelKeyboardProc );
}