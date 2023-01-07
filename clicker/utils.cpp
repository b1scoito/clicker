#include "pch.hpp"
#include "utils.hpp"

namespace string
{
	std::wstring to_unicode( std::string_view str )
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
	void precise_sleep( double secs )
	{
		while (secs > 5e-3)
		{
			auto start = std::chrono::high_resolution_clock::now();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			auto end = std::chrono::high_resolution_clock::now();

			auto observed = (end - start).count() / 1e9;
			secs -= observed;
		}

		/* ~~ spin lock */
		auto start = std::chrono::high_resolution_clock::now();
		while ((std::chrono::high_resolution_clock::now() - start).count() / 1e9 < secs);
	}
}

namespace input
{
	void send_input( mouse_type_t m_type, mouse_side_t m_side )
	{
		POINT pos {};
		if (!GetCursorPos(&pos)) 
			return;

		const auto curr_wnd = GetForegroundWindow();
		
		// SendMessage, SendInput, all poorly optimized for such a precise task
		PostMessage(curr_wnd, (DWORD)m_type, (DWORD)m_side, MAKELPARAM(pos.x, pos.y));
	}

	void click( mouse_input_type_t type, mouse_button_t button )
	{
		(bool) ( type ) ? 
			(bool) ( button ) ? 
			send_input( mouse_type_t::left_down, mouse_side_t::left ):
			send_input( mouse_type_t::right_down, mouse_side_t::right ):
			(bool) ( button ) ? 
			send_input( mouse_type_t::left_up, mouse_side_t::left ):
			send_input( mouse_type_t::right_up, mouse_side_t::right );
	}

	DWORD convert_wm_to_mouseeventf(DWORD m_type)
	{
		switch (m_type)
		{
			case WM_LBUTTONUP:		return MOUSEEVENTF_LEFTUP;
			case WM_LBUTTONDOWN:	return MOUSEEVENTF_LEFTDOWN;
			case WM_RBUTTONUP:		return MOUSEEVENTF_RIGHTUP;
			case WM_RBUTTONDOWN:	return MOUSEEVENTF_RIGHTDOWN;
			default: break;
		}

		return 0x0;
	}
}

namespace focus
{
	std::wstring active_window_title()
	{
		const auto hwnd = GetForegroundWindow();
		if ( !hwnd )
			return {};

		wchar_t title[256];
		GetWindowText( hwnd, title, sizeof( title ) / 4 );

		return title;
	}

	bool is_self_focused()
	{
		const auto hwnd = GetForegroundWindow();
		if ( !hwnd )
			return {};

		DWORD dw_thread_process_id;
		GetWindowThreadProcessId( hwnd, &dw_thread_process_id );

		return dw_thread_process_id && (GetCurrentProcessId() == dw_thread_process_id);
	}

	bool is_cursor_visible()
	{
		CURSORINFO ci { sizeof( CURSORINFO ) };
		if ( !GetCursorInfo( &ci ) )
			return false;

		const auto handle = ci.hCursor;
		if ( ( handle > (HCURSOR) 50000 ) && ( handle < (HCURSOR) 100000 ) )
			return true;

		return false;
	}

	bool window_think()
	{
		if ( !focus::is_self_focused() )
		{
			switch ( config.clicker.i_version_type )
			{
				case 0:
					return GetForegroundWindow() == FindWindow( L"LWJGL", nullptr ) || FindWindow(L"GLFW30", nullptr);
				case 1:
					return active_window_title().find( string::to_unicode( config.clicker.str_window_title ) ) != std::string::npos;
				default:
					return {};
			}
		}

		return false;
	}

	bool cursor_think()
	{
		if ( config.clicker.i_version_type == 0 )
		{
			if ( config.clicker.b_work_in_inventory )
				return !is_cursor_visible() || (vars::key::b_shift_pressed && vars::key::b_inventory_opened && is_cursor_visible() );

			return !is_cursor_visible();
		}

		return true;
	}
}