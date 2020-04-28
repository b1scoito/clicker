#pragma once

#include "includes.hpp"
#include "definitions.hpp" // for font definitions
#include "fa.hpp" // for the font itself.

namespace ui
{
	// d3d9
	static LPDIRECT3D9 direct3d9 = nullptr;
	static LPDIRECT3DDEVICE9 device = nullptr;
	static D3DPRESENT_PARAMETERS parameters = {};

	void render( );
	void activation_type( );
	void create( );

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
	static ImVec4 color( T r, T g, T b, T a = 255 )
	{
		return
		{
			static_cast< float >( r ) / 255.0f,
			static_cast< float >( g ) / 255.0f,
			static_cast< float >( b ) / 255.0f,
			static_cast< float >( a ) / 255.0f
		};
	}

}