#include "pch.hpp"
#include "hooks.hpp"

LRESULT CALLBACK hooks::mouse::m_hook_callback( int nCode, WPARAM wParam, LPARAM lParam )
{
	static auto *m_hook = reinterpret_cast<MSLLHOOKSTRUCT *> (lParam);

	if ((m_hook->flags & LLMHF_INJECTED) || (m_hook->flags & LLMHF_LOWER_IL_INJECTED))
		return false;

	if (nCode == HC_ACTION && wParam != WM_MOUSEMOVE && (wParam >= WM_MOUSEFIRST) && (wParam <= WM_MOUSELAST))
	{
		switch (wParam)
		{
			case WM_LBUTTONDOWN:
				vars::mouse::left_mouse_down = true;
				vars::mouse::left_first_click = true;
				break;
			case WM_LBUTTONUP:
				vars::mouse::left_mouse_down = false;
				break;
			case WM_RBUTTONDOWN:
				vars::mouse::right_mouse_down = true;
				vars::mouse::right_first_click = true;
				break;
			case WM_RBUTTONUP:
				vars::mouse::right_mouse_down = false;
				break;
			default: return CallNextHookEx( hooks::mouse::h_hook, nCode, wParam, lParam );
		}
	}

	return CallNextHookEx( hooks::mouse::h_hook, nCode, wParam, lParam );
}

LRESULT CALLBACK hooks::keyboard::k_hook_callback( int nCode, WPARAM wParam, LPARAM lParam )
{
	static auto *k_hook = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

	if (wParam == WM_KEYDOWN && nCode == HC_ACTION && (wParam >= WM_KEYFIRST) && (wParam <= WM_KEYLAST))
	{
		if (vars::window::is_focused)
		{
			if (k_hook->vkCode == 69)
				vars::key::is_inventory_opened = !vars::key::is_inventory_opened;

			if (k_hook->vkCode == VK_ESCAPE)
				vars::key::is_inventory_opened = false;
		}
	}

	return CallNextHookEx( hooks::keyboard::h_hook, nCode, wParam, lParam );
}

void hooks::init_callbacks()
{
	hooks::mouse::h_hook = SetWindowsHookEx(
		WH_MOUSE_LL,
		hooks::mouse::m_hook_callback,
		nullptr,
		NULL
	);

	hooks::keyboard::h_hook = SetWindowsHookEx(
		WH_KEYBOARD_LL,
		hooks::keyboard::k_hook_callback,
		nullptr,
		NULL
	);

	/* ~~ Both the mouse and keyboard hooks are going to act as your mouse and keyboard,
	 * ~~ so nothing heavy should be executed in those functions. */
	MSG msg {};
	while (GetMessage( &msg, nullptr, 0, 0 ))
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	UnhookWindowsHookEx( hooks::mouse::h_hook );
	UnhookWindowsHookEx( hooks::keyboard::h_hook );
}
