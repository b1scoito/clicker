#pragma once

namespace threads
{
	namespace clicker
	{
		inline void init()
		{
			g_clicker.init();
		}

		inline void randomization()
		{
			g_clicker.update_thread();
		}
	}

	namespace hooking
	{
		inline HHOOK h_hook;

		static LRESULT CALLBACK keyboard_callback( int nCode, WPARAM wParam, LPARAM lParam )
		{
			static auto* k_hook = reinterpret_cast<KBDLLHOOKSTRUCT*>( lParam );

			if ( wParam == WM_KEYDOWN && nCode == HC_ACTION && ( wParam >= WM_KEYFIRST ) && ( wParam <= WM_KEYLAST ) )
			{
				if ( util::extra::is_window_focused() )
				{
					if ( k_hook->vkCode == 69 /* E */ )
						vars::key::inventory_opened = !vars::key::inventory_opened;

					if ( k_hook->vkCode == VK_ESCAPE )
						vars::key::inventory_opened = false;
				}
			}

			return CallNextHookEx( h_hook, nCode, wParam, lParam );
		}

		inline void spawn()
		{
			h_hook = SetWindowsHookEx(
				WH_KEYBOARD_LL,
				keyboard_callback,
				nullptr,
				NULL
			);

			MSG msg;
			while ( GetMessage( &msg, nullptr, 0, 0 ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}

		inline void unhook()
		{
			UnhookWindowsHookEx( h_hook );
		}
	}
}