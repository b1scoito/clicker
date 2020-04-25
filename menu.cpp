#pragma once

#include "menu.hpp"

void menu::render( )
{
	ImGui::NewFrame( );
	{
		ImGui::Begin( xorstr( "clicker" ), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar );
		{
			ImGui::SetWindowSize( ( ImVec2( 500, 300 ) ) );
			ImGui::SetWindowPos( ImVec2( -1, -1 ) );

			ImGui::Checkbox( xorstr( "enabled" ), &config.clicker.enabled );

			ImGui::SliderFloat( xorstr( "maximum cps" ), &config.clicker.max_cps, 1.f, 20.f, xorstr( "%.1f" ) );

			if ( ImGui::IsItemHovered( ) )
				ImGui::SetTooltip( xorstr( "maximum clicks per second" ) );

			ImGui::SliderFloat( xorstr( "minimum cps" ), &config.clicker.min_cps, 1.f, 20.f, xorstr( "%.1f" ) );

			if ( ImGui::IsItemHovered( ) )
				ImGui::SetTooltip( xorstr( "minimum clicks per second" ) );

			ImGui::Combo( xorstr( "version" ), &config.clicker.index_version, vars::items, IM_ARRAYSIZE( vars::items ) );

			static char buffer[ 16 ];

			// this is the worst logic pls help

			switch ( config.clicker.index_version )
			{
				case 0:
					config.clicker.window_name = "Minecraft";
					break;
				case 1:
					config.clicker.window_name = "Badlion";
					break;
				case 2:
					config.clicker.window_name = "Lunar";
					break;
				case 3:
					ImGui::InputText( xorstr( "window name" ), buffer, IM_ARRAYSIZE( buffer ) );
					config.clicker.window_name = buffer;
					break;
			}

			/* bad math */
			if ( config.clicker.max_cps <= config.clicker.min_cps )
				config.clicker.max_cps += 1.f;

			ImGui::Text( xorstr( "is button down: %s" ), vars::b_mouse_down ? "yes" : "no" );
			ImGui::Text( xorstr( "current cps: %d" ), vars::d_current_cps );
			ImGui::Text( xorstr( "average %.3f ms/framerate (%.1f fps)" ), 1000.0f / ImGui::GetIO( ).Framerate, ImGui::GetIO( ).Framerate );

			ImGui::BeginChild( xorstr( "config" ), ImVec2( 250, 250 ) );
			{
				constexpr auto &config_items = config.get_configs( );
				static int current_config = -1;

				if ( static_cast< size_t >( current_config ) >= config_items.size( ) )
					current_config = -1;

				static char buffer[ 16 ];

				if ( ImGui::ListBox( xorstr( "configs" ), &current_config, [ ]( void *data, int idx, const char **out_text )
					{
						auto &vector = *static_cast< std::vector<std::string> * >( data );
						*out_text = vector[ idx ].c_str( );
						return true;
					}, &config_items, config_items.size( ), 5 ) && current_config != -1 )
					strcpy_s( buffer, config_items[ current_config ].c_str( ) );

					if ( ImGui::InputText( xorstr( "config name" ), buffer, IM_ARRAYSIZE( buffer ), ImGuiInputTextFlags_EnterReturnsTrue ) )
					{
						if ( current_config != -1 )
							config.rename( current_config, buffer );
					}

					if ( ImGui::Button( ( xorstr( "create" ) ), ImVec2( 75, 20 ) ) )
					{
						config.add( buffer );
					}

					ImGui::SameLine( );

					if ( ImGui::Button( ( xorstr( "reset" ) ), ImVec2( 75, 20 ) ) )
					{
						config.reset( );
					}

					if ( current_config != -1 )
					{
						if ( ImGui::Button( ( xorstr( "load" ) ), ImVec2( 75, 20 ) ) )
						{
							config.load( current_config );
						}

						ImGui::SameLine( );

						if ( ImGui::Button( ( xorstr( "save" ) ), ImVec2( 75, 20 ) ) )
						{
							config.save( current_config );
						}

						if ( ImGui::Button( ( xorstr( "remove" ) ), ImVec2( 75, 20 ) ) )
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
	WNDCLASSEX wc = { sizeof( WNDCLASSEX ), CS_CLASSDC, menu::wndproc, 0L, 0L, GetModuleHandle( nullptr ), nullptr, nullptr, nullptr, nullptr, xorstr( "clicker" ) , nullptr };
	::RegisterClassEx( &wc );
	HWND hwnd = ::CreateWindow( wc.lpszClassName, xorstr( "" ), /* no resizing */ WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 100, 100, /* ghetto */ 515, 338, nullptr, nullptr, wc.hInstance, nullptr );

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

	io.IniFilename = nullptr; // no config file

	ImGui::StyleColorsDark( );

	style->WindowRounding = 0.0f;
	style->FrameRounding = 3.0f;
	style->GrabRounding = 10.0f;

	ImGui_ImplWin32_Init( hwnd );
	ImGui_ImplDX9_Init( menu::device );

	ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

	_log( LINFO, xorstr( "waiting for program end." ) );

	MSG lpMsg;
	ZeroMemory( &lpMsg, sizeof( lpMsg ) );

	while ( lpMsg.message != WM_QUIT )
	{
		if ( ::PeekMessage( &lpMsg, nullptr, 0U, 0U, PM_REMOVE ) )
		{
			::TranslateMessage( &lpMsg );
			::DispatchMessage( &lpMsg );
			continue;
		}

		ImGui_ImplDX9_NewFrame( );
		ImGui_ImplWin32_NewFrame( );

		// rending menu
		menu::render( );

		menu::device->SetRenderState( D3DRS_ZENABLE, false );
		menu::device->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
		menu::device->SetRenderState( D3DRS_SCISSORTESTENABLE, false );

		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA( ( int ) ( clear_color.x * 255.0f ), ( int ) ( clear_color.y * 255.0f ), ( int ) ( clear_color.z * 255.0f ), ( int ) ( clear_color.w * 255.0f ) );
		menu::device->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );

		if ( menu::device->BeginScene( ) >= 0 )
		{
			ImGui::Render( );
			ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
			menu::device->EndScene( );
		}

		HRESULT result = menu::device->Present( nullptr, nullptr, nullptr, nullptr );

		if ( result == D3DERR_DEVICELOST && menu::device->TestCooperativeLevel( ) == D3DERR_DEVICENOTRESET )
			menu::reset_device( );
	}

	// destroy imgui and window and d3d and my thighs
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
	if ( ( direct3d9 = Direct3DCreate9( D3D_SDK_VERSION ) ) == nullptr )
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
		device = nullptr;
	}

	if ( direct3d9 )
	{
		direct3d9->Release( );
		direct3d9 = nullptr;
	}
}

void menu::reset_device( )
{
	ImGui_ImplDX9_InvalidateDeviceObjects( );

	HRESULT result = device->Reset( &parameters );

	if ( result == D3DERR_INVALIDCALL )
		IM_ASSERT( 0 );

	ImGui_ImplDX9_CreateDeviceObjects( );
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT __stdcall menu::wndproc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, wParam, lParam ) )
		return true;

	switch ( msg )
	{
		case WM_SIZE:
			if ( device != nullptr && wParam != SIZE_MINIMIZED )
			{
				parameters.BackBufferWidth = LOWORD( lParam );
				parameters.BackBufferHeight = HIWORD( lParam );
				menu::reset_device( );
			}
			return 0;
		case WM_SYSCOMMAND:
			if ( ( wParam & 0xfff0 ) == SC_KEYMENU )
				return 0;
			break;
		case WM_DESTROY:
			::PostQuitMessage( 0 );
			return 0;
	}
	return ::DefWindowProc( hwnd, msg, wParam, lParam );
}