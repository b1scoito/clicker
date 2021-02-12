#pragma once

#include "../includes.hpp"
#include "../def/include/imgui/fonts/font_definitions.hpp" // for font definitions
#include "../def/include/imgui/fonts/fontawesome.hpp" // for the font itself.

namespace ui
{
	// d3d9
	static LPDIRECT3D9 direct3d9 = nullptr;
	static LPDIRECT3DDEVICE9 device = nullptr;
	static D3DPRESENT_PARAMETERS parameters = {};

	void render( );

	inline std::string get_key_name_by_id( int id )
	{
		static std::unordered_map< int, std::string > key_names =
		{
			{ 0, xorstr( "None" ) },
			{ VK_LBUTTON, xorstr( "Mouse 1" ) },
			{ VK_RBUTTON, xorstr( "Mouse 2" ) },
			{ VK_MBUTTON, xorstr( "Mouse 3" ) },
			{ VK_XBUTTON1, xorstr( "Mouse 4" ) },
			{ VK_XBUTTON2, xorstr( "Mouse 5" ) },
			{ VK_BACK, xorstr( "Back" ) },
			{ VK_TAB, xorstr( "Tab" ) },
			{ VK_CLEAR, xorstr( "Clear" ) },
			{ VK_RETURN, xorstr( "Enter" ) },
			{ VK_SHIFT, xorstr( "Shift" ) },
			{ VK_CONTROL, xorstr( "Ctrl" ) },
			{ VK_MENU, xorstr( "Alt" ) },
			{ VK_PAUSE, xorstr( "Pause" ) },
			{ VK_CAPITAL, xorstr( "Caps Lock" ) },
			{ VK_ESCAPE, xorstr( "Escape" ) },
			{ VK_SPACE, xorstr( "Space" ) },
			{ VK_PRIOR, xorstr( "Page Up" ) },
			{ VK_NEXT, xorstr( "Page Down" ) },
			{ VK_END, xorstr( "End" ) },
			{ VK_HOME, xorstr( "Home" ) },
			{ VK_LEFT, xorstr( "Left Key" ) },
			{ VK_UP, xorstr( "Up Key" ) },
			{ VK_RIGHT, xorstr( "Right Key" ) },
			{ VK_DOWN, xorstr( "Down Key" ) },
			{ VK_SELECT, xorstr( "Select" ) },
			{ VK_PRINT, xorstr( "Print Screen" ) },
			{ VK_INSERT, xorstr( "Insert" ) },
			{ VK_DELETE, xorstr( "Delete" ) },
			{ VK_HELP, xorstr( "Help" ) },
			{ VK_SLEEP, xorstr( "Sleep" ) },
			{ VK_MULTIPLY, xorstr( "*" ) },
			{ VK_ADD, xorstr( "+" ) },
			{ VK_SUBTRACT, xorstr( "-" ) },
			{ VK_DECIMAL, xorstr( "." ) },
			{ VK_DIVIDE, xorstr( "/" ) },
			{ VK_NUMLOCK, xorstr( "Num Lock" ) },
			{ VK_SCROLL, xorstr( "Scroll" ) },
			{ VK_LSHIFT, xorstr( "Left Shift" ) },
			{ VK_RSHIFT, xorstr( "Right Shift" ) },
			{ VK_LCONTROL, xorstr( "Left Ctrl" ) },
			{ VK_RCONTROL, xorstr( "Right Ctrl" ) },
			{ VK_LMENU, xorstr( "Left Alt" ) },
			{ VK_RMENU, xorstr( "Right Alt" ) },
		};

		if ( id >= 0x30 && id <= 0x5A )
			return std::string( 1, ( char ) id );

		if ( id >= 0x60 && id <= 0x69 )
			return xorstr( "Num " ) + std::to_string( id - 0x60 );

		if ( id >= 0x70 && id <= 0x87 )
			return xorstr( "F" ) + std::to_string( ( id - 0x70 ) + 1 );

		return key_names[ id ];
	}

	void key_bind_button( int &key, int width, int height );

	void activation_type( );

	bool create( );

	void dispose( HWND hwnd, WNDCLASSEX wc );

	namespace d3d9
	{
		inline bool create_device_d3d( HWND hwnd )
		{
			if ( ( direct3d9 = Direct3DCreate9( D3D_SDK_VERSION ) ) == nullptr )
				return false;

			ZeroMemory( &parameters, sizeof( parameters ) );

			parameters.Windowed = TRUE;
			parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
			parameters.BackBufferFormat = D3DFMT_UNKNOWN;
			parameters.EnableAutoDepthStencil = TRUE;
			parameters.AutoDepthStencilFormat = D3DFMT_D16;
			parameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

			if ( direct3d9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &parameters, &device ) < 0 )
				return false;

			return true;
		}

		inline void cleanup_device_d3d( )
		{
			if ( device )
			{
				device->Release( );
				device = nullptr;
			}

			if ( direct3d9 )
			{
				direct3d9->Release( );
				direct3d9 = nullptr;
			}
		}

		inline void reset_device( )
		{
			ImGui_ImplDX9_InvalidateDeviceObjects( );

			HRESULT result = device->Reset( &parameters );

			if ( result == D3DERR_INVALIDCALL )
				IM_ASSERT( 0 );

			ImGui_ImplDX9_CreateDeviceObjects( );
		}

		LRESULT __stdcall wndproc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
	}

	template <typename T>
	ImVec4 color( T r, T g, T b, T a = 255 )
	{
		return ImColor( r, g, b, a );
	}

}