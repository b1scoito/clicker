#pragma once

#include <unordered_map>
#include <string>

#include <d3d9.h>
#include <shellapi.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include "imgui/font_definitions.h"
#include "imgui/font_data.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

class c_menu
{
private:
	void on_paint();

	ImVec4 float_to_imvec4( float* color ) const { return ImVec4( color[0], color[1], color[2], color[3] ); }

	void keybind_button( int& i_key, int i_width, int i_height );
	std::string get_key_name_by_id( int id );

	void set_position( int x, int y, int cx, int cy, HWND hwnd );
	void get_mouse_offset( int& x, int& y, HWND hwnd );

private:
	static LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	bool CreateDeviceD3D( HWND hWnd );
	void CleanupDeviceD3D();
	static void ResetDevice();

public:
	~c_menu() = default;
	c_menu() = default;

	bool setup();
	void render();
	void destroy();
};

inline auto g_menu = std::make_unique<c_menu>();