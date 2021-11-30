#pragma once

namespace threads
{
	// Find a better way to sort this out
	inline void spawn_clicker() { g_clicker->init(); }
	inline void spawn_rand() { g_clicker->update_thread(); }

	inline HHOOK oLowLevelMouseProc = nullptr;
	inline HHOOK oLowLevelKeyboardProc = nullptr;

	extern LRESULT CALLBACK LowLevelMouseProc( int nCode, WPARAM wParam, LPARAM lParam );
	extern LRESULT CALLBACK LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam );

	void destroy();
	DWORD WINAPI spawn_hooks();
}