#include "mouse.hpp"

LRESULT CALLBACK m_hook( int nCode, WPARAM wParam, LPARAM lParam )
{
	auto *hook = reinterpret_cast< MSLLHOOKSTRUCT * > ( lParam );

	if ( ( hook->flags == LLMHF_INJECTED ) )
	{
		hook->flags &= ~LLMHF_INJECTED;
		hook->flags &= ~LLMHF_LOWER_IL_INJECTED;
		return false;
	}

	if ( wParam != WM_MOUSEMOVE )
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

	return CallNextHookEx( g_mouse->hk_mouse, nCode, wParam, lParam );
}

void mouse::work( )
{
	g_mouse->hk_mouse = SetWindowsHookExA( WH_MOUSE_LL, &m_hook, nullptr, 0 );

	MSG lpMsg;
	while ( GetMessage( &lpMsg, nullptr, 0, 0 ) )
	{
		TranslateMessage( &lpMsg );
		DispatchMessageA( &lpMsg );
	}

	UnhookWindowsHookEx( g_mouse->hk_mouse );
}

void mouse::keybind_work( )
{
	while ( true )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
		g_mouse->activation_type( );
	}
}

void mouse::activation_type( )
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