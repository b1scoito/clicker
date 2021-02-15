#pragma once

#include "../def/imgui/imgui.h"
#include "../def/imgui/impl/imgui_impl_dx9.h"
#include "../def/imgui/impl/imgui_impl_win32.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "../def/imgui/fonts/font_definitions.hpp"
#include "../def/imgui/fonts/fontawesome.hpp"

#include "../def/includes.hpp"

#define DIRECTINPUT_VERSION 0x0800

class menu
{
public:
	void render_objects( HWND hwnd, int width, int height );
	bool create( int width, int height );

private:
	template <typename T>
	ImVec4 color( T r, T g, T b, T a = 255 )
	{
		return ImColor( r, g, b, a );
	}

	void key_bind_button( int &key, int width, int height )
	{
		static auto b_get = false;
		static std::string sz_text = "Click to bind";

		if ( ImGui::Button( sz_text.c_str( ), ImVec2( static_cast< float >( width ), static_cast< float >( height ) ) ) )
			b_get = true;

		if ( b_get )
		{
			for ( auto i = 1; i < 256; i++ )
			{
				if ( GetAsyncKeyState( i ) & 0x8000 )
				{
					if ( i != 12 )
					{
						key = i == VK_ESCAPE ? 0 : i;
						b_get = false;
					}
				}
			}
			sz_text = "Press a key";
		}
		else if ( !b_get && key == 0 )
			sz_text = "Click to bind";
		else if ( !b_get && key != 0 )
			sz_text = "Bound to " + util::to_lower( get_key_name_by_id( key ) );
	}

	std::string get_key_name_by_id( int id )
	{
		static std::unordered_map< int, std::string > key_names = {
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
			return std::string( 1, ( char ) id );

		if ( id >= 0x60 && id <= 0x69 )
			return "Num " + std::to_string( id - 0x60 );

		if ( id >= 0x70 && id <= 0x87 )
			return "F" + std::to_string( ( id - 0x70 ) + 1 );

		return key_names[ id ];
	}

	void activation_type( )
	{
		switch ( config.clicker.activation_type )
		{
			case 0:
				if ( config.clicker.left_enabled || config.clicker.right_enabled )
					config.clicker.hotkey_enabled = true;
				break;

			case 1:
				if ( GetAsyncKeyState( config.clicker.key ) )
					config.clicker.hotkey_enabled = true;
				else
					config.clicker.hotkey_enabled = false;
				break;

			case 2:
				if ( GetAsyncKeyState( config.clicker.key ) )
				{
					vars::b_is_clicked = false;
					vars::b_is_down = true;
				}
				else if ( !GetAsyncKeyState( config.clicker.key ) && vars::b_is_down )
				{
					vars::b_is_clicked = true;
					vars::b_is_down = false;
				}
				else
				{
					vars::b_is_clicked = false;
					vars::b_is_down = false;
				}

				if ( vars::b_is_clicked )
					config.clicker.hotkey_enabled = !config.clicker.hotkey_enabled;

				break;

		}
	}

public:
	LPDIRECT3D9 g_pD3D = NULL;
	LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
	D3DPRESENT_PARAMETERS g_d3dpp = {};


	void set_position( int x, int y, int w, int h, bool b_center, HWND hwnd )
	{
		POINT point;
		GetCursorPos( &point );

		auto flags = SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE;
		if ( x != 0 && y != 0 )
		{
			x = point.x - x;
			y = point.y - y;
			flags &= ~SWP_NOMOVE;
		}

		if ( w != 0 && h != 0 )
			flags &= ~SWP_NOSIZE;

		if ( b_center )
		{
			RECT rect;
			if ( w != 0 && h != 0 )
			{
				rect.right = w;
				rect.bottom = h;
			}
			else
				GetWindowRect( hwnd, &rect );

			x = ( GetSystemMetrics( SM_CXSCREEN ) - rect.right ) / 2;
			y = ( GetSystemMetrics( SM_CYSCREEN ) - rect.bottom ) / 2;

			flags &= ~SWP_NOMOVE;
		}

		SetWindowPos( hwnd, nullptr, x, y, w, h, flags );
	}

	void get_mouse_offset( int &x, int &y, HWND hwnd )
	{
		POINT point;
		RECT rect;

		GetCursorPos( &point );
		GetWindowRect( hwnd, &rect );

		x = point.x - rect.left;
		y = point.y - rect.top;
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

	void cleanup_device_d3d( )
	{
		if ( g_pd3dDevice ) { g_pd3dDevice->Release( ); g_pd3dDevice = NULL; }
		if ( g_pD3D ) { g_pD3D->Release( ); g_pD3D = NULL; }
	}

	void reset_device( )
	{
		ImGui_ImplDX9_InvalidateDeviceObjects( );

		HRESULT hr = g_pd3dDevice->Reset( &g_d3dpp );
		if ( hr == D3DERR_INVALIDCALL )
			IM_ASSERT( 0 );

		ImGui_ImplDX9_CreateDeviceObjects( );
	}
};


inline auto g_menu = std::make_unique<menu>( );