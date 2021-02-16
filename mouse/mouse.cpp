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