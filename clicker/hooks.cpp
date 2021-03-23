#include "pch.hpp"
#include "hooks.hpp"

LRESULT CALLBACK hooks::mouse::m_hook_callback( int nCode, WPARAM wParam, LPARAM lParam )
{
	auto *m_hook = reinterpret_cast<MSLLHOOKSTRUCT *> (lParam);

	if ((m_hook->flags & LLMHF_INJECTED) || (m_hook->flags & LLMHF_LOWER_IL_INJECTED))
	{
		m_hook->flags &= ~LLMHF_INJECTED;
		m_hook->flags &= ~LLMHF_LOWER_IL_INJECTED;

		return false;
	}

	if (nCode == HC_ACTION && wParam != WM_MOUSEMOVE)
	{
		switch (wParam)
		{
			case WM_LBUTTONDOWN:
				var::mouse::left_first_click = true;
				var::mouse::left_mouse_down = true;
				break;
			case WM_LBUTTONUP:
				var::mouse::left_mouse_down = false;
				break;
			case WM_RBUTTONDOWN:
				var::mouse::right_first_click = true;
				var::mouse::right_mouse_down = true;
				break;
			case WM_RBUTTONUP:
				var::mouse::right_mouse_down = false;
				break;
			default: break;
		}
	}

	return CallNextHookEx( hooks::mouse::h_hook, nCode, wParam, lParam );
}

LRESULT CALLBACK hooks::keyboard::k_hook_callback( int nCode, WPARAM wParam, LPARAM lParam )
{
	auto *k_hook = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

	auto in_window = util::other::get_active_window_title().find( config.clicker.window_title ) != std::string::npos;

	if (nCode == HC_ACTION && wParam == WM_KEYDOWN)
	{
		if (in_window)
		{
			if (k_hook->vkCode == 69) /*E*/
				var::key::is_inventory_opened = !var::key::is_inventory_opened;

			if (k_hook->vkCode == VK_ESCAPE)
				var::key::is_inventory_opened = false;
		}
	}

	return CallNextHookEx( hooks::keyboard::h_hook, nCode, wParam, lParam );
}

void hooks::init_callbacks()
{
	hooks::mouse::h_hook = SetWindowsHookEx
	(
		WH_MOUSE_LL,
		hooks::mouse::m_hook_callback,
		nullptr,
		NULL
	);

	hooks::keyboard::h_hook = SetWindowsHookEx
	(
		WH_KEYBOARD_LL,
		hooks::keyboard::k_hook_callback,
		nullptr,
		NULL
	);

	// If this thread hangs up for processing other things, the mouse will skip in-game, because everything mouse-related passes through this hook.
	// I personally think there's no fix for this.
	MSG msg {};
	while (GetMessage( &msg, nullptr, 0, 0 ))
	{
		if (msg.message == WM_QUIT)
			break;

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	UnhookWindowsHookEx( hooks::mouse::h_hook );
	UnhookWindowsHookEx( hooks::keyboard::h_hook );
}

void hooks::keybinds::init()
{
	while (true)
	{
		hooks::keybinds::activation_type();
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}
}

// Let the gambiarra begin
void hooks::keybinds::activation_type()
{
	switch (config.clicker.activation_type)
	{
		case 0:
			if (config.clicker.left_enabled || config.clicker.right_enabled)
				var::key::is_hotkey_enabled = true;
			else
				var::key::is_hotkey_enabled = false;
			break;

		case 1:
			if (GetAsyncKeyState( config.clicker.key ))
				var::key::is_hotkey_enabled = true;
			else
				var::key::is_hotkey_enabled = false;
			break;

		case 2:
			if (GetAsyncKeyState( config.clicker.key ))
			{
				var::key::is_clicked = false;
				var::key::is_down = true;
			}
			else if (!GetAsyncKeyState( config.clicker.key ) && var::key::is_down)
			{
				var::key::is_clicked = true;
				var::key::is_down = false;
			}
			else
			{
				var::key::is_clicked = false;
				var::key::is_down = false;
			}

			if (var::key::is_clicked)
				var::key::is_hotkey_enabled = !var::key::is_hotkey_enabled;

			break;
	}
}