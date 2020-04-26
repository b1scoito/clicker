#include "util.hpp"

HHOOK hhk_mouse;

LRESULT __stdcall util::hooking::hh_mouse_callback( int nCode, WPARAM wParam, LPARAM lParam )
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

	return LI_FN( CallNextHookEx ).safe_cached( )( hhk_mouse, nCode, wParam, lParam );
}

DWORD __stdcall util::hooking::m_hook_all( )
{
	hhk_mouse = LI_FN( SetWindowsHookExA ).safe_cached( )( WH_MOUSE_LL, &util::hooking::hh_mouse_callback, nullptr, 0 );

	MSG lpMsg;
	while ( LI_FN( GetMessageA ).safe_cached( )( &lpMsg, nullptr, 0, 0 ) )
	{
		LI_FN( TranslateMessage ).safe_cached( )( &lpMsg );
		LI_FN( DispatchMessageA ).safe_cached( )( &lpMsg );
	}

	LI_FN( UnhookWindowsHookEx ).safe_cached( )( hhk_mouse );

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

std::string util::get_active_window_title( )
{
	char title[ 256 ];
	HWND hwnd = LI_FN( GetForegroundWindow ).safe_cached( )( );
	LI_FN( GetWindowTextA ).safe_cached( )( hwnd, title, sizeof( title ) );
	return title;
}

std::string util::random_string( int i_size )
{
	static auto &chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	thread_local static std::mt19937 rg { std::random_device{}( ) };
	thread_local static std::uniform_int_distribution<std::string::size_type> pick( 0, sizeof ( chars ) - 2 );

	std::string string;
	string.reserve( i_size );

	while ( i_size-- )
		string += chars[ pick( rg ) ];

	return string;
}

int util::random_int( int i_start, int i_end )
{
	std::random_device rd;
	std::mt19937 rng( rd( ) );
	const std::uniform_int_distribution<int> uni( i_start, i_end );

	return static_cast< int >( uni( rng ) );
}