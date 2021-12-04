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

static LPDIRECT3D9 g_pD3D;
static LPDIRECT3DDEVICE9 g_pd3dDevice;
static D3DPRESENT_PARAMETERS g_d3dpp;

class c_menu
{
private:
	ImVec4 float_to_imvec4( float* color ) const { return ImVec4( color[0], color[1], color[2], color[3] ); }

	void on_paint( HWND hwnd, int i_width, int i_height );

	void keybind_button( int& i_key, int i_width, int i_height );
	std::string get_key_name_by_id( int id );

	void set_position( int x, int y, int cx, int cy, HWND hwnd );
	void get_mouse_offset( int& x, int& y, HWND hwnd );

	static LRESULT WINAPI wndproc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	bool create_device_d3d( HWND hWnd );
	void cleanup_device_d3d();
	static void reset_device();
public:
	~c_menu() = default;
	c_menu() = default;

	bool init( int width, int height ) noexcept;
};

inline auto g_menu = std::make_unique<c_menu>();