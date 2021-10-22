#pragma once

namespace threads
{
	namespace clicker
	{
		inline void spawn()
		{
			g_clicker.init();
		}
	}

	namespace randomization
	{
		inline void spawn()
		{
			g_clicker.update_thread();
		}
	}

	namespace hooks
	{
		inline HHOOK h_hook;

		static LRESULT CALLBACK keyboard_callback( int nCode, WPARAM wParam, LPARAM lParam )
		{
			static auto* k_hook = (KBDLLHOOKSTRUCT*) ( lParam );

			if ( wParam == WM_KEYDOWN && nCode == HC_ACTION && ( wParam >= WM_KEYFIRST ) && ( wParam <= WM_KEYLAST ) )
			{
				if ( focus::window_think() )
				{
					if ( k_hook->vkCode == 69 /* E */ )
						vars::key::b_inventory_opened = !vars::key::b_inventory_opened;

					if ( k_hook->vkCode == VK_ESCAPE )
						vars::key::b_inventory_opened = false;
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