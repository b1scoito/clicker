#include "util.hpp"

LRESULT __stdcall util::hooking::mouse( int nCode, WPARAM wParam, LPARAM lParam )
{
	auto *hook = reinterpret_cast< MSLLHOOKSTRUCT * > ( lParam );

	if ( ( hook->flags == LLMHF_INJECTED ) ) // Don't inject on me please.
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

	return CallNextHookEx( hook_mouse, nCode, wParam, lParam );
}

DWORD __stdcall util::hooking::work( )
{
	hook_mouse = SetWindowsHookExA( WH_MOUSE_LL, &util::hooking::mouse, nullptr, 0 );

	MSG lpMsg;
	while ( GetMessage( &lpMsg, nullptr, 0, 0 ) )
	{
		TranslateMessage( &lpMsg );
		DispatchMessageA( &lpMsg );
	}

	UnhookWindowsHookEx( hook_mouse );

	return EXIT_SUCCESS;
}

void util::input::left_down( )
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput( 1, &input, sizeof( INPUT ) );
}

void util::input::left_up( )
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput( 1, &input, sizeof( INPUT ) );
}

void util::input::right_down( )
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput( 1, &input, sizeof( INPUT ) );
}

void util::input::right_up( )
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput( 1, &input, sizeof( INPUT ) );
}

std::string util::to_lower( std::string str )
{
	std::transform( str.begin( ), str.end( ), str.begin( ), static_cast< int( * )( int ) >( ::tolower ) );
	return str;
}

int util::random_int( int i_start, int i_end )
{
	std::random_device rd;
	std::mt19937 rng( rd( ) );
	const std::uniform_int_distribution<int> uni( i_start, i_end );

	return static_cast< int >( uni( rng ) );
}

std::string util::get_active_window_title( )
{
	char title[ 256 ] = { 0 };
	HWND hwnd = GetForegroundWindow( );
	GetWindowTextA( hwnd, title, sizeof( title ) );
	return title;
}

std::string util::get_serial( )
{
	DWORD disk_serial;
	GetVolumeInformationA( R"(C:)", nullptr, 0, &disk_serial, nullptr, nullptr, nullptr, 0 );

	return std::to_string( disk_serial );
}