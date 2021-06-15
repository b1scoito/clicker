#pragma once

namespace util
{
	namespace string
	{
		inline std::string to_upper( std::string str )
		{
			std::transform( str.begin(), str.end(), str.begin(), static_cast<int( * )( int )>( ::toupper ) );
			return str;
		}

		inline std::string to_lower( std::string str )
		{
			std::transform( str.begin(), str.end(), str.begin(), static_cast<int( * )( int )>( ::tolower ) );
			return str;
		}

		inline std::wstring to_upper( std::wstring wstr )
		{
			std::transform( wstr.begin(), wstr.end(), wstr.begin(), static_cast<int( * )( int )>( ::toupper ) );
			return wstr;
		}

		inline std::wstring to_lower( std::wstring wstr )
		{
			std::transform( wstr.begin(), wstr.end(), wstr.begin(), static_cast<int( * )( int )>( ::tolower ) );
			return wstr;
		}

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

	namespace random
	{
		inline float number( float f_start, float f_end )
		{
			static std::mt19937 mersenne { static_cast<std::mt19937::result_type>( std::time( {} ) ) };
			const std::uniform_real_distribution<float> distribution( f_start, f_end );
			return distribution( mersenne );
		}
	}

	namespace extra
	{
		inline std::wstring get_active_window_title()
		{
			wchar_t title[256];
			const auto hwnd = GetForegroundWindow();
			GetWindowText( hwnd, title, sizeof( title ) / 4 );
			return title;
		}

		inline bool is_application_focused()
		{
			const auto hwnd = GetForegroundWindow();
			if ( !hwnd )
				return false;

			DWORD dw_thread_process_id;
			GetWindowThreadProcessId( hwnd, &dw_thread_process_id );
			return ( GetCurrentProcessId() == dw_thread_process_id );
		}

		inline bool cursor_visible()
		{
			CURSORINFO ci { sizeof( CURSORINFO ) };
			if ( GetCursorInfo( &ci ) )
			{
				const auto handle = ci.hCursor;

				if ( !( ( handle > (HCURSOR) 50000 ) & ( handle < (HCURSOR) 100000 ) ) )
					return true;
			}

			return false;
		}

		inline bool is_window_focused()
		{
			switch ( config.clicker.i_version_type )
			{
				case 0:
					return ( GetForegroundWindow() == FindWindow( L"LWJGL", nullptr ) );
					break;
				case 1:
					return ( get_active_window_title().find( util::string::to_unicode( config.clicker.str_window_title ) ) != std::string::npos );
					break;
			}

			return false;
		}

		inline bool is_cursor_visible()
		{
			if ( config.clicker.b_only_in_game )
			{
				if ( config.clicker.b_work_in_inventory )
				{
					return cursor_visible() || ( vars::key::inventory_opened && !cursor_visible() );
				}

				return cursor_visible();
			}

			return false;
		}
	}
}