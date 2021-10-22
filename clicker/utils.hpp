#pragma once

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

namespace math
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
	// TODO: More compact and simple single function to send inputs.
	inline void left_up()
	{
		INPUT ip = {};
		{
			ip.type = INPUT_MOUSE;
			ip.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		}

		SendInput( 1, &ip, sizeof( INPUT ) );
	}

	inline void left_down()
	{
		INPUT ip = {};
		{
			ip.type = INPUT_MOUSE;
			ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		}

		SendInput( 1, &ip, sizeof( INPUT ) );
	}

	inline void right_up()
	{
		INPUT ip = {};
		{
			ip.type = INPUT_MOUSE;
			ip.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		}

		SendInput( 1, &ip, sizeof( INPUT ) );
	}

	inline void right_down()
	{
		INPUT ip = {};
		{
			ip.type = INPUT_MOUSE;
			ip.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		}

		SendInput( 1, &ip, sizeof( INPUT ) );
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
			return {};

		const auto handle = ci.hCursor;

		if ( ( handle > (HCURSOR) 50000 ) & ( handle < (HCURSOR) 100000 ) )
			return true;

		return {};
	}

	inline bool window_think()
	{
		switch ( config.clicker.i_version_type )
		{
			case 0: return ( GetForegroundWindow() == FindWindow( L"LWJGL", nullptr ) );
			case 1: return ( active_window_title().find( string::to_unicode( config.clicker.str_window_title ) ) != std::string::npos );
		}

		return {};
	}

	inline bool cursor_think()
	{
		if ( config.clicker.b_only_in_game )
		{
			if ( config.clicker.b_work_in_inventory )
				return !is_cursor_visible() || ( vars::key::b_inventory_opened && is_cursor_visible() );

			return !is_cursor_visible();
		}

		return {};
	}
}