#pragma once

#include <winternl.h>

namespace string
{
	inline std::string to_utf8( std::wstring wstr )
	{
		if ( wstr.empty() )
			return {};

		const auto size = WideCharToMultiByte( CP_UTF8, 0, wstr.data(), (int) wstr.size(), 0, 0, 0, 0 );
		auto ret = std::string( size, 0 );

		WideCharToMultiByte( CP_UTF8, 0, wstr.data(), (int) wstr.size(), ret.data(), size, 0, 0 );

		return ret;
	}

	inline std::wstring to_unicode( std::string str )
	{
		if ( str.empty() )
			return {};

		const auto size = MultiByteToWideChar( CP_UTF8, 0, str.data(), (int) str.size(), 0, 0 );
		auto ret = std::wstring( size, 0 );

		MultiByteToWideChar( CP_UTF8, 0, str.data(), (int) str.size(), ret.data(), size );

		return ret;
	}
}

namespace timer
{
	inline void precise_sleep(float ms)
	{
		static HMODULE ntdll = NULL;
		if (!ntdll)
			ntdll = GetModuleHandle(L"ntdll.dll");

		typedef NTSTATUS(WINAPI* fnNtDelayExecution)(BOOL Alertable, PLARGE_INTEGER DelayInterval);
		static fnNtDelayExecution oNtDelayExecution = NULL;

		if (!oNtDelayExecution)
			oNtDelayExecution = (fnNtDelayExecution)GetProcAddress(ntdll, "NtDelayExecution");

		typedef NTSTATUS(WINAPI* fnZwSetTimerResolution)(IN ULONG RequestedResolution, IN BOOLEAN Set, OUT PULONG ActualResolution);
		static fnZwSetTimerResolution oZwSetTimerResolution = NULL;

		if (!oZwSetTimerResolution)
			oZwSetTimerResolution = (fnZwSetTimerResolution)GetProcAddress(ntdll, "ZwSetTimerResolution");

		static std::once_flag flag;
		std::call_once(flag, []()
			{
				ULONG current;
				oZwSetTimerResolution((ULONG)(0.5f * 10000.f), true, &current);
			});

		if (ms < 0.5f)
			ms = 0.5f;

		LARGE_INTEGER time = { 0 };
		time.QuadPart = -1 * (LONGLONG)(ms * 10000.f);

		oNtDelayExecution(false, &time);
	}
}

namespace rng
{
	template <typename T>
	T random_range( T start, T end )
	{
		std::random_device rd;
		std::mt19937_64 rng( rd() );
		std::uniform_int_distribution<T> uni( start, end );

		return (T) uni( rng );
	}

	template <typename T>
	T random_real( T start, T end )
	{
		std::random_device rd;
		std::mt19937_64 rng( rd() );
		std::uniform_real_distribution<T> uni( start, end );

		return (T) uni( rng );
	}
}

namespace input
{
	// TODO: Melhorar isso tudo
	enum class mouse_button_t: bool { right, left };
	enum class mouse_input_type_t: bool { up, down };
	enum class mouse_type_t : DWORD
	{
		left_up = MOUSEEVENTF_LEFTUP,
		left_down = MOUSEEVENTF_LEFTDOWN,

		right_up = MOUSEEVENTF_RIGHTUP,
		right_down = MOUSEEVENTF_RIGHTDOWN
	};
	
	inline void send_input(mouse_type_t m_type)
	{
		INPUT ip = {};
		{
			ip.type = INPUT_MOUSE;
			ip.mi.dwFlags = (DWORD)m_type;
		}

		SendInput(1, &ip, sizeof(INPUT));
	}

	inline void click(mouse_input_type_t type, mouse_button_t button)
	{
		(bool)(type) ? (bool)(button) ? send_input(mouse_type_t::left_down) : send_input(mouse_type_t::right_down) : (bool)(button) ? send_input(mouse_type_t::left_up) : send_input(mouse_type_t::right_up);
	}

}

namespace focus
{
	inline std::wstring active_window_title()
	{
		const auto hwnd = GetForegroundWindow();
		if ( !hwnd )
			return {};

		wchar_t title[256];
		GetWindowText( hwnd, title, sizeof( title ) / 4 );

		return title;
	}

	inline bool is_self_focused()
	{
		const auto hwnd = GetForegroundWindow();
		if ( !hwnd )
			return {};

		DWORD dw_thread_process_id;
		GetWindowThreadProcessId( hwnd, &dw_thread_process_id );

		return ( GetCurrentProcessId() == dw_thread_process_id );
	}

	inline bool is_cursor_visible()
	{
		CURSORINFO ci { sizeof( CURSORINFO ) };
		if ( !GetCursorInfo( &ci ) )
			return false;

		const auto handle = ci.hCursor;

		if ( ( handle > (HCURSOR) 50000 ) & ( handle < (HCURSOR) 100000 ) )
			return true;

		return false;
	}

	inline bool window_think()
	{
		switch ( config.clicker.i_version_type )
		{
			case 0: 
				return ( GetForegroundWindow() == FindWindow( L"LWJGL", nullptr ) );
			case 1: 
				return ( active_window_title().find( string::to_unicode( config.clicker.str_window_title ) ) != std::string::npos );
			default: break;
		}

		return false;
	}

	inline bool cursor_think()
	{
		if ( config.clicker.b_only_in_game )
		{
			if ( config.clicker.b_work_in_inventory )
				return !is_cursor_visible() || ( vars::key::b_inventory_opened && is_cursor_visible() );

			return !is_cursor_visible();
		}
		else
			return is_cursor_visible();

		return false;
	}
}