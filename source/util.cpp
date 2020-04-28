#include "util.hpp"

LRESULT __stdcall util::hooking::mouse( int nCode, WPARAM wParam, LPARAM lParam )
{
	MSLLHOOKSTRUCT *hook = ( MSLLHOOKSTRUCT * ) lParam;

	if ( ( hook->flags == LLMHF_INJECTED ) || ( hook->flags == LLMHF_LOWER_IL_INJECTED ) )
		return false;

	if ( ( hook->flags & LLMHF_INJECTED ) == LLMHF_INJECTED )
		return false;

	if ( wParam != WM_MOUSEMOVE )
	{
		if ( ( hook->flags == LLMHF_INJECTED ) || ( hook->flags == LLMHF_LOWER_IL_INJECTED ) )
			return false;

		switch ( wParam )
		{
			case WM_LBUTTONDOWN:
				var::b_first_click = true;
				var::b_mouse_down = true;
				break;
			case WM_LBUTTONUP:
				var::b_mouse_down = false;
				break;
		}
	}

	return LI_FN( CallNextHookEx ).safe_cached( )( hook_mouse, nCode, wParam, lParam );
}

DWORD __stdcall util::hooking::work( )
{
	hook_mouse = LI_FN( SetWindowsHookExA ).safe_cached( )( WH_MOUSE_LL, &util::hooking::mouse, nullptr, 0 );

	MSG lpMsg;
	while ( LI_FN( GetMessageA ).safe_cached( )( &lpMsg, nullptr, 0, 0 ) )
	{
		LI_FN( TranslateMessage ).safe_cached( )( &lpMsg );
		LI_FN( DispatchMessageA ).safe_cached( )( &lpMsg );
	}

	LI_FN( UnhookWindowsHookEx ).safe_cached( )( hook_mouse );

	return EXIT_SUCCESS;
}

void util::input::left_down( )
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	LI_FN( SendInput ).safe_cached( )( 1, &input, sizeof( INPUT ) );
}

void util::input::left_up( )
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	LI_FN( SendInput ).safe_cached( )( 1, &input, sizeof( INPUT ) );
}