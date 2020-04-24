#pragma once

#include "includes.hpp"

namespace menu
{
	// d3d9
	static LPDIRECT3D9              direct3d9 = nullptr;
	static LPDIRECT3DDEVICE9        device = nullptr;
	static D3DPRESENT_PARAMETERS    parameters = {};

	void render( );
	bool initialize( );

	bool create_device_d3d( HWND hWnd );
	void cleanup_device_d3d( );
	void reset_device( );

	LRESULT WINAPI wndproc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
}