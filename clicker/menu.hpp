#pragma once

#include <d3d9.h>
#include <unordered_map>
#include <string> // w

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/font_definitions.h"
#include "imgui/font_data.h"

static LPDIRECT3D9 g_pD3D;
static LPDIRECT3DDEVICE9 g_pd3dDevice;
static D3DPRESENT_PARAMETERS g_d3dpp;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

class c_menu
{
private:
	ImVec4 float_to_imvec4( float* color ) const { return ImVec4( color[0], color[1], color[2], color[3] ); }

	// ~ render ImGui objects
	void on_paint( HWND hwnd, int i_width, int i_height );

	// ~ ImGui widget for keybind button
	void keybind_button( int& i_key, int i_width, int i_height )
	{
		static auto b_get = false;
		static std::string sz_text( "Click to bind" );

		if ( ImGui::Button( sz_text.c_str(), ImVec2( static_cast<float>( i_width ), static_cast<float>( i_height ) ) ) )
			b_get = true;

		if ( b_get )
		{
			for ( auto i = 1; i < 256; i++ )
			{
				if ( GetAsyncKeyState( i ) & 0x8000 )
				{
					if ( i != 12 )
					{
						i_key = i == VK_ESCAPE ? 0 : i;
						b_get = false;
					}
				}
			}
			sz_text = "Press a key";
		}
		else if ( !b_get && i_key == 0 )
			sz_text = "Click to bind";
		else if ( !b_get && i_key != 0 )
			sz_text = "Bound to " + get_key_name_by_id( i_key );
	}

	// ~ returns a key name from a keycode
	std::string get_key_name_by_id( int id )
	{
		static std::unordered_map<int, std::string> key_names = {
			{ 0, "None" },
			{ VK_LBUTTON, "Mouse 1" },
			{ VK_RBUTTON, "Mouse 2" },
			{ VK_MBUTTON, "Mouse 3" },
			{ VK_XBUTTON1, "Mouse 4" },
			{ VK_XBUTTON2, "Mouse 5" },
			{ VK_BACK, "Back" },
			{ VK_TAB, "Tab" },
			{ VK_CLEAR, "Clear" },
			{ VK_RETURN, "Enter" },
			{ VK_SHIFT, "Shift" },
			{ VK_CONTROL, "Ctrl" },
			{ VK_MENU, "Alt" },
			{ VK_PAUSE, "Pause" },
			{ VK_CAPITAL, "Caps Lock" },
			{ VK_ESCAPE, "Escape" },
			{ VK_SPACE, "Space" },
			{ VK_PRIOR, "Page Up" },
			{ VK_NEXT, "Page Down" },
			{ VK_END, "End" },
			{ VK_HOME, "Home" },
			{ VK_LEFT, "Left Key" },
			{ VK_UP, "Up Key" },
			{ VK_RIGHT, "Right Key" },
			{ VK_DOWN, "Down Key" },
			{ VK_SELECT, "Select" },
			{ VK_PRINT, "Print Screen" },
			{ VK_INSERT, "Insert" },
			{ VK_DELETE, "Delete" },
			{ VK_HELP, "Help" },
			{ VK_SLEEP, "Sleep" },
			{ VK_MULTIPLY, "*" },
			{ VK_ADD, "+" },
			{ VK_SUBTRACT, "-" },
			{ VK_DECIMAL, "." },
			{ VK_DIVIDE, "/" },
			{ VK_NUMLOCK, "Num Lock" },
			{ VK_SCROLL, "Scroll" },
			{ VK_LSHIFT, "Left Shift" },
			{ VK_RSHIFT, "Right Shift" },
			{ VK_LCONTROL, "Left Ctrl" },
			{ VK_RCONTROL, "Right Ctrl" },
			{ VK_LMENU, "Left Alt" },
			{ VK_RMENU, "Right Alt" },
		};

		if ( id >= 0x30 && id <= 0x5A )
			return std::string( 1, (char) id );

		if ( id >= 0x60 && id <= 0x69 )
			return "Num " + std::to_string( id - 0x60 );

		if ( id >= 0x70 && id <= 0x87 )
			return "F" + std::to_string( ( id - 0x70 ) + 1 );

		return key_names[id];
	}

	// ~ sets a window handle position
	void set_position( int x, int y, int cx, int cy, HWND hwnd )
	{
		POINT point; GetCursorPos( &point );

		auto flags = SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE;
		if ( x != 0 && y != 0 )
		{
			x = point.x - x;
			y = point.y - y;
			flags &= ~SWP_NOMOVE;
		}

		if ( cx != 0 && cy != 0 )
			flags &= ~SWP_NOSIZE;

		SetWindowPos( hwnd, nullptr, x, y, cx, cy, flags );
	}

	// ~ gets the mouse offset position
	void get_mouse_offset( int& x, int& y, HWND hwnd )
	{
		POINT point; RECT rect;

		GetCursorPos( &point );
		GetWindowRect( hwnd, &rect );

		x = point.x - rect.left;
		y = point.y - rect.top;
	}

	static LRESULT WINAPI wnd_proc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
			return true;

		switch ( msg )
		{
			case WM_SIZE:
				if ( g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED )
				{
					g_d3dpp.BackBufferWidth = LOWORD( lParam );
					g_d3dpp.BackBufferHeight = HIWORD( lParam );
					reset_device();
				}
				return 0;
			case WM_SYSCOMMAND:
				if ( ( wParam & 0xfff0 ) == SC_KEYMENU )
					return 0;
				break;
			case WM_DESTROY:
				PostQuitMessage( 0 );
				return 0;
		}

		return DefWindowProc( hWnd, msg, wParam, lParam );
	}

	bool create_device_d3d( HWND hWnd )
	{
		if ( ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL )
			return false;

		ZeroMemory( &g_d3dpp, sizeof( g_d3dpp ) );

		g_d3dpp.Windowed = TRUE;
		g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		g_d3dpp.EnableAutoDepthStencil = TRUE;
		g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

		if ( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice ) < 0 )
			return false;

		return true;
	}

	void cleanup_device_d3d()
	{
		if ( g_pd3dDevice ) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
		if ( g_pD3D ) { g_pD3D->Release(); g_pD3D = NULL; }
	}

	static void reset_device()
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();

		HRESULT hr = g_pd3dDevice->Reset( &g_d3dpp );
		if ( hr == D3DERR_INVALIDCALL )
			IM_ASSERT( 0 );

		ImGui_ImplDX9_CreateDeviceObjects();
	}
public:
	~c_menu() = default;
	c_menu() = default;

	__forceinline bool initialize( int width, int height ) noexcept
	{
		const WNDCLASSEX wc = {
			sizeof( WNDCLASSEX ), CS_CLASSDC,
			this->wnd_proc,
			0L, 0L,
			GetModuleHandle( NULL ),
			NULL, NULL, NULL, NULL,
			L"Class",
			NULL
		};

		RegisterClassEx( &wc );

		const auto hwnd = CreateWindow(
			wc.lpszClassName,
			L"",
			WS_POPUP,
			100, 100,
			width, height,
			NULL, NULL,
			wc.hInstance,
			NULL
		);

		if ( !this->create_device_d3d( hwnd ) )
		{
			this->cleanup_device_d3d();
			UnregisterClass( wc.lpszClassName, wc.hInstance );

			return false;
		}

		ShowWindow( hwnd, SW_SHOWDEFAULT );
		UpdateWindow( hwnd );

		ImGui::CreateContext();

		auto& io = ImGui::GetIO();
		auto& style = ImGui::GetStyle();

		if ( PWSTR fonts_path; SUCCEEDED( SHGetKnownFolderPath( FOLDERID_Fonts, 0, nullptr, &fonts_path ) ) )
		{
			const std::filesystem::path path { fonts_path };
			CoTaskMemFree( fonts_path );
			io.Fonts->AddFontFromFileTTF( ( path / "SegoeUI.ttf" ).string().c_str(), 16.f, NULL, io.Fonts->GetGlyphRangesDefault() );
		}

		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;

		io.Fonts->AddFontFromMemoryCompressedTTF( fontawesome_compressed_data, fontawesome_compressed_size, 10.f, &icons_config, icons_ranges );

		style.ScrollbarSize = 10.0f;
		style.GrabRounding = 5.0f;
		style.GrabMinSize = 10.0f;
		style.FrameRounding = 3.0f;
		style.TabRounding = 3.0f;

		io.IniFilename = nullptr;

		ImGui_ImplWin32_Init( hwnd );
		ImGui_ImplDX9_Init( g_pd3dDevice );

		const auto clear_color = ImVec4( 0.09f, 0.09f, 0.09f, 0.94f );

		auto done = false;
		while ( !done )
		{
			MSG msg;
			while ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );

				if ( msg.message == WM_QUIT )
					done = true;
			}

			if ( done )
				break;

			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();

			ImGui::NewFrame();

			auto* colors = style.Colors;
			colors[ImGuiCol_Text] = this->float_to_imvec4( config.clicker.f_color_accent_text );
			colors[ImGuiCol_WindowBg] = ImVec4( 0.11f, 0.11f, 0.11f, 0.94f );
			colors[ImGuiCol_PopupBg] = ImVec4( 0.11f, 0.11f, 0.11f, 0.94f );
			colors[ImGuiCol_Border] = this->float_to_imvec4( config.clicker.f_color_accent );
			colors[ImGuiCol_FrameBg] = ImVec4( 0.15f, 0.15f, 0.15f, 0.54f );
			colors[ImGuiCol_TitleBgActive] = this->float_to_imvec4( config.clicker.f_color_accent );
			colors[ImGuiCol_FrameBgHovered] = ImVec4( 0.19f, 0.19f, 0.19f, 0.54f );
			colors[ImGuiCol_FrameBgActive] = ImVec4( 0.26f, 0.26f, 0.26f, 0.54f );
			colors[ImGuiCol_ScrollbarBg] = ImVec4( 0.11f, 0.11f, 0.11f, 0.94f );
			colors[ImGuiCol_ScrollbarGrab] = this->float_to_imvec4( config.clicker.f_color_accent );
			colors[ImGuiCol_TextSelectedBg] = this->float_to_imvec4( config.clicker.f_color_accent );
			colors[ImGuiCol_CheckMark] = this->float_to_imvec4( config.clicker.f_color_accent );
			colors[ImGuiCol_SliderGrab] = this->float_to_imvec4( config.clicker.f_color_accent );
			colors[ImGuiCol_SliderGrabActive] = this->float_to_imvec4( config.clicker.f_color_accent_active );
			colors[ImGuiCol_Button] = this->float_to_imvec4( config.clicker.f_color_accent );
			colors[ImGuiCol_ButtonHovered] = this->float_to_imvec4( config.clicker.f_color_accent_hovered );
			colors[ImGuiCol_ButtonActive] = this->float_to_imvec4( config.clicker.f_color_accent_active );
			colors[ImGuiCol_Header] = this->float_to_imvec4( config.clicker.f_color_accent );
			colors[ImGuiCol_HeaderHovered] = this->float_to_imvec4( config.clicker.f_color_accent_hovered );
			colors[ImGuiCol_HeaderActive] = this->float_to_imvec4( config.clicker.f_color_accent_active );
			colors[ImGuiCol_Separator] = this->float_to_imvec4( config.clicker.f_color_accent );
			colors[ImGuiCol_Tab] = this->float_to_imvec4( config.clicker.f_color_accent );
			colors[ImGuiCol_TabHovered] = this->float_to_imvec4( config.clicker.f_color_accent_hovered );
			colors[ImGuiCol_TabActive] = this->float_to_imvec4( config.clicker.f_color_accent_active );

			this->on_paint( hwnd, width, height );

			ImGui::EndFrame();

			g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
			g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			g_pd3dDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
			D3DCOLOR clear_col_dx = D3DCOLOR_RGBA( (int) ( clear_color.x * clear_color.w * 255.0f ), (int) ( clear_color.y * clear_color.w * 255.0f ), (int) ( clear_color.z * clear_color.w * 255.0f ), (int) ( clear_color.w * 255.0f ) );
			g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );

			if ( g_pd3dDevice->BeginScene() >= 0 )
			{
				ImGui::Render();
				ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );
				g_pd3dDevice->EndScene();
			}

			HRESULT result = g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

			if ( result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET )
				this->reset_device();
		}

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();

		ImGui::DestroyContext();

		this->cleanup_device_d3d();

		DestroyWindow( hwnd );

		UnregisterClass( wc.lpszClassName, wc.hInstance );

		return 0;
	}
};

inline auto g_menu = c_menu();