#pragma once

namespace util
{
	namespace string
	{
		inline auto to_upper( std::string str ) -> std::string
		{
			std::transform( str.begin(), str.end(), str.begin(), static_cast<int( * )( int )>( ::toupper ) );
			return str;
		}

		inline auto to_lower( std::string str ) -> std::string
		{
			std::transform( str.begin(), str.end(), str.begin(), static_cast<int( * )( int )>( ::tolower ) );
			return str;
		}

		inline auto to_upper( std::wstring wstr ) -> std::wstring
		{
			std::transform( wstr.begin(), wstr.end(), wstr.begin(), static_cast<int( * )( int )>( ::toupper ) );
			return wstr;
		}

		inline auto to_lower( std::wstring wstr ) -> std::wstring
		{
			std::transform( wstr.begin(), wstr.end(), wstr.begin(), static_cast<int( * )( int )>( ::tolower ) );
			return wstr;
		}

		inline auto to_utf8( std::wstring wstr ) -> std::string
		{
			if ( wstr.empty() )
				return {};

			const auto size = WideCharToMultiByte( CP_UTF8, 0, wstr.data(), wstr.size(), 0, 0, 0, 0 );
			auto ret = std::string( size, 0 );

			WideCharToMultiByte( CP_UTF8, 0, wstr.data(), wstr.size(), ret.data(), size, 0, 0 );

			return ret;
		}

		inline auto to_unicode( std::string str ) -> std::wstring
		{
			if ( str.empty() )
				return {};

			const auto size = MultiByteToWideChar( CP_UTF8, 0, str.data(), str.size(), 0, 0 );
			auto ret = std::wstring( size, 0 );

			MultiByteToWideChar( CP_UTF8, 0, str.data(), str.size(), ret.data(), size );

			return ret;
		}
	}

	namespace random
	{
		inline auto number( float start, float end ) -> float
		{
			static std::mt19937 mersenne { static_cast<std::mt19937::result_type>( std::time( {} ) ) };
			const std::uniform_real_distribution<float> distribution( start, end );
			return distribution( mersenne );
		}
	}

	namespace extra
	{
		inline auto get_active_window_title() -> std::wstring
		{
			wchar_t title[256];
			auto hwnd = GetForegroundWindow();
			GetWindowText( hwnd, title + 1, sizeof( title ) );
			return title;
		}

		inline auto is_application_focused() -> bool
		{
			auto hwnd = GetForegroundWindow();
			if ( !hwnd )
				return {};

			DWORD dw_thread_process_id;
			GetWindowThreadProcessId( hwnd, &dw_thread_process_id );
			return ( GetCurrentProcessId() == dw_thread_process_id );
		}

		inline auto cursor_handle_status() -> bool
		{
			CURSORINFO ci { sizeof( CURSORINFO ) };

			if ( GetCursorInfo( &ci ) )
			{
				auto handle = ci.hCursor;

				if ( ( handle > (HCURSOR) 50000 ) & ( handle < (HCURSOR) 100000 ) )
					return true;
			}

			return false;
		}

		inline auto is_window_focused() -> bool
		{
			switch ( config.clicker.i_version_type )
			{
				case 0:
					return ( GetForegroundWindow() == FindWindow( L"LWJGL", nullptr ) );
				case 1:
					return get_active_window_title().find( util::string::to_unicode( config.clicker.window_title ) ) != std::string::npos;
				default:
					return {};
			}

			return false;
		}

		inline auto is_cursor_visible() -> bool
		{
			if ( config.clicker.b_only_in_game )
			{
				if ( config.clicker.b_work_in_inventory )
					return !( cursor_handle_status() ) || ( var::key::inventory_opened && cursor_handle_status() );

				return !( cursor_handle_status() );
			}

			return true;
		}
	}
}