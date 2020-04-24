#pragma once

#include "menu.hpp"

void menu::render( )
{
	ImGui::NewFrame( );
	{
		ImGui::Begin( xorstr( "clicker" ), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse );
		{
			ImGui::SetWindowSize( ( ImVec2( 500, 300 ) ) );
			ImGui::SetWindowPos( ImVec2( -1, -1 ) );

			ImGui::Checkbox( "enabled", &config.clicker.enabled );

			ImGui::SliderFloat( xorstr( "minimum cps" ), &config.clicker.min_cps, 1.f, 20.f, xorstr( "%.2f" ) );

			if ( ImGui::IsItemHovered( ) )
				ImGui::SetTooltip( xorstr( "minimum clicks per second" ) );

			ImGui::SliderFloat( xorstr( "maximum cps" ), &config.clicker.max_cps, 1.f, 20.f, xorstr( "%.2f" ) );

			if ( ImGui::IsItemHovered( ) )
				ImGui::SetTooltip( xorstr( "maximum clicks per second" ) );

			ImGui::Text( xorstr( "is button down: %s" ), vars::b_mouse_down ? "yes" : "no" );
			ImGui::Text( xorstr( "application average %.3f ms/framerate (%.1f FPS)" ), 1000.0f / ImGui::GetIO( ).Framerate, ImGui::GetIO( ).Framerate );

			ImGui::BeginChild( "config", ImVec2( 250, 250 ) );
			{
				constexpr auto &config_items = config.get_configs( );
				static int current_config = -1;

				if ( static_cast< size_t >( current_config ) >= config_items.size( ) )
					current_config = -1;

				static char buffer[ 16 ];

				if ( ImGui::ListBox( "configs", &current_config, [ ]( void *data, int idx, const char **out_text )
					{
						auto &vector = *static_cast< std::vector<std::string> * >( data );
						*out_text = vector[ idx ].c_str( );
						return true;
					}, &config_items, config_items.size( ), 5 ) && current_config != -1 )
					strcpy_s( buffer, config_items[ current_config ].c_str( ) );


					if ( ImGui::InputText( "", buffer, IM_ARRAYSIZE( buffer ), ImGuiInputTextFlags_EnterReturnsTrue ) )
					{
						if ( current_config != -1 )
							config.rename( current_config, buffer );
					}

					if ( ImGui::Button( ( "create" ), ImVec2( 85, 20 ) ) )
					{
						config.add( buffer );
					}

					ImGui::SameLine( );

					if ( ImGui::Button( ( "reset" ), ImVec2( 85, 20 ) ) )
					{
						config.reset( );
					}

					if ( current_config != -1 )
					{
						if ( ImGui::Button( ( "load" ), ImVec2( 85, 20 ) ) )
						{
							config.load( current_config );
						}

						ImGui::SameLine( );

						if ( ImGui::Button( ( "save" ), ImVec2( 85, 20 ) ) )
						{
							config.save( current_config );
						}

						if ( ImGui::Button( ( "remove" ), ImVec2( 85, 20 ) ) )
						{
							config.remove( current_config );
						}
					}
			}
			ImGui::EndChild( );
		}
		ImGui::End( );
	}
	ImGui::EndFrame( );
}

bool menu::initialize( )
{
	WNDCLASSEX wc = { sizeof( WNDCLASSEX ), CS_CLASSDC, menu::wndproc, 0L, 0L, GetModuleHandle( NULL ), NULL, NULL, NULL, NULL, xorstr( "clicker" ) , NULL };
	::RegisterClassEx( &wc );
	HWND hwnd = ::CreateWindow( wc.lpszClassName, xorstr( "" ), WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 100, 100, 500, 300, NULL, NULL, wc.hInstance, NULL );

	if ( !menu::create_device_d3d( hwnd ) )
	{
		menu::cleanup_device_d3d( );
		::UnregisterClass( wc.lpszClassName, wc.hInstance );
		return EXIT_FAILURE;
	}

	::ShowWindow( hwnd, SW_SHOWDEFAULT );
	::UpdateWindow( hwnd );

	IMGUI_CHECKVERSION( );
	ImGui::CreateContext( );

	ImGuiStyle *style = &ImGui::GetStyle( );

	ImGuiIO &io = ImGui::GetIO( ); ( void ) io;

	ImGui::StyleColorsDark( );

	style->WindowRounding = 0.0f;
	style->FrameRounding = 5.0f;
	style->GrabRounding = 5.0f;

	ImGui_ImplWin32_Init( hwnd );
	ImGui_ImplDX9_Init( menu::device );

	bool show_demo_window = false;
	ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

	_log( LINFO, xorstr( "waiting for program end." ) );

	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );
	while ( msg.message != WM_QUIT )
	{
		if ( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
			continue;
		}

		ImGui_ImplDX9_NewFrame( );
		ImGui_ImplWin32_NewFrame( );

		menu::render( );

		menu::device->SetRenderState( D3DRS_ZENABLE, FALSE );
		menu::device->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		menu::device->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );

		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA( ( int ) ( clear_color.x * 255.0f ), ( int ) ( clear_color.y * 255.0f ), ( int ) ( clear_color.z * 255.0f ), ( int ) ( clear_color.w * 255.0f ) );
		menu::device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );

		if ( menu::device->BeginScene( ) >= 0 )
		{
			ImGui::Render( );
			ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
			menu::device->EndScene( );
		}
		HRESULT result = menu::device->Present( NULL, NULL, NULL, NULL );

		if ( result == D3DERR_DEVICELOST && menu::device->TestCooperativeLevel( ) == D3DERR_DEVICENOTRESET )
			menu::reset_device( );
	}

	ImGui_ImplDX9_Shutdown( );
	ImGui_ImplWin32_Shutdown( );
	ImGui::DestroyContext( );

	menu::cleanup_device_d3d( );
	::DestroyWindow( hwnd );
	::UnregisterClass( wc.lpszClassName, wc.hInstance );

	return EXIT_SUCCESS;
}

bool menu::create_device_d3d( HWND hWnd )
{
	if ( ( direct3d9 = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL )
		return false;

	ZeroMemory( &parameters, sizeof( parameters ) );
	parameters.Windowed = TRUE;
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	parameters.BackBufferFormat = D3DFMT_UNKNOWN;
	parameters.EnableAutoDepthStencil = TRUE;
	parameters.AutoDepthStencilFormat = D3DFMT_D16;
	parameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if ( direct3d9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &parameters, &device ) < 0 )
		return false;

	return true;
}

void menu::cleanup_device_d3d( )
{
	if ( device )
	{
		device->Release( );
		device = NULL;
	}

	if ( direct3d9 )
	{
		direct3d9->Release( );
		direct3d9 = NULL;
	}
}

void menu::reset_device( )
{
	ImGui_ImplDX9_InvalidateDeviceObjects( );
	HRESULT hr = device->Reset( &parameters );

	if ( hr == D3DERR_INVALIDCALL )
		IM_ASSERT( 0 );

	ImGui_ImplDX9_CreateDeviceObjects( );
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT WINAPI menu::wndproc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
		return true;

	switch ( msg )
	{
		case WM_SIZE:
			if ( device != NULL && wParam != SIZE_MINIMIZED )
			{
				parameters.BackBufferWidth = LOWORD( lParam );
				parameters.BackBufferHeight = HIWORD( lParam );
				reset_device( );
			}
			return 0;
		case WM_SYSCOMMAND:
			if ( ( wParam & 0xfff0 ) == SC_KEYMENU )
				return 0;
			break;
		case WM_DESTROY:
			::PostQuitMessage( 0 );
			return 0;
		case WM_ACTIVATE:
			if ( LOWORD( wParam ) == WA_ACTIVE )
			{
				_log( LDEBUG, "bruh." );
			}
			break;
	}
	return ::DefWindowProc( hWnd, msg, wParam, lParam );
}