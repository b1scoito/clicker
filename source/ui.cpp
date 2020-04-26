#pragma once

#include "ui.hpp"

void ui::render( )
{
	ImGui::NewFrame( );
	{
		ImGui::Begin( xorstr( "clicker" ), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar );
		{
			ImGui::SetWindowSize( ( ImVec2( 500, 300 ) ) );
			ImGui::SetWindowPos( ImVec2( -1, -1 ) );

			ImGui::Checkbox( xorstr( "enabled" ), &config.clicker.enabled );

			if ( ImGui::IsItemHovered( ) )
				ImGui::SetTooltip( xorstr( "master switch" ) );

			ImGui::SliderFloat( xorstr( "maximum cps" ), &config.clicker.max_cps, 1.f, 20.f, xorstr( "%.1f" ) );

			if ( ImGui::IsItemHovered( ) )
				ImGui::SetTooltip( xorstr( "maximum clicks per second" ) );

			ImGui::SliderFloat( xorstr( "minimum cps" ), &config.clicker.min_cps, 1.f, 20.f, xorstr( "%.1f" ) );

			if ( ImGui::IsItemHovered( ) )
				ImGui::SetTooltip( xorstr( "minimum clicks per second" ) );

			ImGui::Combo( xorstr( "version" ), &config.clicker.index_version, var::items, IM_ARRAYSIZE( var::items ) );

			if ( ImGui::IsItemHovered( ) )
				ImGui::SetTooltip( xorstr( "window that the auto clicker will work in" ) );

			// this is the worst logic pls help

			static char buffer[ 16 ];

			switch ( config.clicker.index_version )
			{
				case 0:
					config.clicker.window_title = "Minecraft";
					break;
				case 1:
					config.clicker.window_title = "Badlion";
					break;
				case 2:
					config.clicker.window_title = "Lunar";
					break;
				case 3:
					ImGui::InputText( xorstr( "window title" ), buffer, IM_ARRAYSIZE( buffer ) );
					config.clicker.window_title = buffer;
					break;
			}

			/* bad math */
			if ( config.clicker.max_cps <= config.clicker.min_cps )
				config.clicker.max_cps += 1.f;

			ImGui::Text( xorstr( "is button down: %s" ), var::b_mouse_down ? "yes" : "no" );

			if ( ImGui::IsItemHovered( ) )
				ImGui::SetTooltip( xorstr( "checks whether or not the left button is pressed down" ) );

			ImGui::Text( xorstr( "current cps: %d" ), var::i_current_cps );

			if ( ImGui::IsItemHovered( ) )
				ImGui::SetTooltip( xorstr( "current clicks per second" ) );

			ImGui::Text( xorstr( "clicks on this session %d" ), var::i_clicks_this_session );

			if ( ImGui::IsItemHovered( ) )
				ImGui::SetTooltip( xorstr( "clicks on the current session" ) );

			ImGui::Text( xorstr( "application average %.3f ms/framerate (%.1f fps)" ), 1000.0f / ImGui::GetIO( ).Framerate, ImGui::GetIO( ).Framerate );

			constexpr auto &config_items = config.get_configs( );
			static int current_config = -1;

			if ( static_cast< size_t >( current_config ) >= config_items.size( ) )
				current_config = -1;

			static char buffer_config[ 16 ];

			if ( ImGui::ListBox( xorstr( "configs" ), &current_config, [ ]( void *data, int idx, const char **out_text )
				{
					auto &vector = *static_cast< std::vector<std::string> * >( data );
					*out_text = vector[ idx ].c_str( );
					return true;
				}, &config_items, config_items.size( ), 5 ) && current_config != -1 )
				strcpy_s( buffer_config, config_items[ current_config ].c_str( ) );

				if ( ImGui::InputText( xorstr( "name" ), buffer_config, IM_ARRAYSIZE( buffer_config ), ImGuiInputTextFlags_EnterReturnsTrue ) )
				{
					if ( current_config != -1 )
						config.rename( current_config, buffer_config );
				}

				if ( ImGui::Button( ( xorstr( "create" ) ), ImVec2( 75, 20 ) ) )
				{
					config.add( buffer_config );
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

					if ( ImGui::Button( ( xorstr( "delete" ) ), ImVec2( 75, 20 ) ) )
					{
						config.remove( current_config );
					}
				}
		}
		ImGui::End( );
	}
	ImGui::EndFrame( );
}

void ui::initialize( )
{
	WNDCLASSEX wc =
	{
		sizeof( WNDCLASSEX ),
		CS_CLASSDC,
		ui::d3d9::wndproc,
		0L,
		0L,
		LI_FN( GetModuleHandleA ).safe_cached( )( nullptr ),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		xorstr( "pop_rsi_r18" ),
		nullptr
	};

	LI_FN( RegisterClassExA ).safe_cached( )( &wc );

	HWND hwnd = CreateWindowA
	(
		wc.lpszClassName,
		xorstr( "" ),
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		100,
		100,
		515,
		338,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	if ( !ui::d3d9::create_device_d3d( hwnd ) )
	{
		ui::d3d9::cleanup_device_d3d( );
		LI_FN( UnregisterClassA ).safe_cached( )( wc.lpszClassName, wc.hInstance );
	}

	LI_FN( ShowWindow ).safe_cached( )( hwnd, SW_SHOWDEFAULT );
	LI_FN( UpdateWindow ).safe_cached( )( hwnd );

	ImGui::CreateContext( );

	ImGuiStyle *style = &ImGui::GetStyle( );
	ImGuiIO &io = ImGui::GetIO( ); ( void ) io;

	io.IniFilename = nullptr; // no config file

	ImGui::StyleColorsDark( );

	style->WindowRounding = 0.0f;
	style->FrameRounding = 3.0f;
	style->GrabRounding = 10.0f;

	ImGui_ImplWin32_Init( hwnd );
	ImGui_ImplDX9_Init( device );

	ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

	_log( LINFO, xorstr( "imgui pre init" ) );

	MSG lpMsg;
	ZeroMemory( &lpMsg, sizeof( lpMsg ) );

	while ( lpMsg.message != WM_QUIT )
	{
		if ( LI_FN( PeekMessageA ).safe_cached( )( &lpMsg, nullptr, 0U, 0U, PM_REMOVE ) )
		{
			LI_FN( TranslateMessage ).safe_cached( )( &lpMsg );
			LI_FN( DispatchMessageA ).safe_cached( )( &lpMsg );
			continue;
		}

		ImGui_ImplDX9_NewFrame( );
		ImGui_ImplWin32_NewFrame( );

		// rending menu
		ui::render( );

		device->SetRenderState( D3DRS_ZENABLE, false );
		device->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
		device->SetRenderState( D3DRS_SCISSORTESTENABLE, false );
		
		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA
		(
			( int ) ( clear_color.x * 255.0f ),
			( int ) ( clear_color.y * 255.0f ),
			( int ) ( clear_color.z * 255.0f ),
			( int ) ( clear_color.w * 255.0f )
		);

		device->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );

		if ( device->BeginScene( ) >= 0 )
		{
			ImGui::Render( );
			ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
			device->EndScene( );
		}

		HRESULT result = device->Present( nullptr, nullptr, nullptr, nullptr );

		if ( result == D3DERR_DEVICELOST && device->TestCooperativeLevel( ) == D3DERR_DEVICENOTRESET )
			ui::d3d9::reset_device( );
	}

	// destroy imgui and window n d3d and my thighs ;3
	ImGui_ImplDX9_Shutdown( );
	ImGui_ImplWin32_Shutdown( );

	ImGui::DestroyContext( );

	ui::d3d9::cleanup_device_d3d( );

	LI_FN( DestroyWindow ).safe_cached( )( hwnd );
	LI_FN( UnregisterClassA ).safe_cached( )( wc.lpszClassName, wc.hInstance );

}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT __stdcall ui::d3d9::wndproc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
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

				ui::d3d9::reset_device( );
			}
			return 0;

		case WM_SYSCOMMAND:
			if ( ( wParam & 0xfff0 ) == SC_KEYMENU )
				return 0;
			break;

		case WM_DESTROY:
			LI_FN( PostQuitMessage ).safe_cached( )( 0 );
			return 0;
	}

	return LI_FN( DefWindowProcA ).safe_cached( )( hwnd, msg, wParam, lParam );
}