#pragma once

#include "ui.hpp"

void ui::render( )
{
	ImGui::NewFrame( );
	{
		ImGui::Begin( xorstr( "clicker" ), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar );
		{
			ImGui::SetWindowSize( ImVec2( 534, 311 ) );
			ImGui::SetWindowPos( ImVec2( 0, 0 ) );

			ImGui::Checkbox( xorstr( "##enabled" ), &config.clicker.enabled );

			ImGui::SameLine( );

			ImGui::Combo( xorstr( "##combo" ), &config.clicker.activation_type, xorstr( "enabled\0hold\0toggle\0\0" ) );

			ImGui::SameLine( );

			ui::key_bind_button( config.clicker.enabled_key, 125, 20 );

			ui::activation_type( );

			ImGui::SliderInt( xorstr( "##mincps" ), &config.clicker.max_cps, 1, 20, xorstr( "min cps %d" ) );
			ImGui::SliderInt( xorstr( "##maxcps" ), &config.clicker.min_cps, 1, 20, xorstr( "max cps %d" ) );

			ImGui::Combo( xorstr( "version" ), &config.clicker.version_type, xorstr( "Badlion " ICON_FA_CHECK "\0Lunar " ICON_FA_CHECK "\0Minecraft\0Custom\0\0" ) ); // thanks imgui

			if ( ImGui::IsItemHovered( ) )
				ImGui::SetTooltip( xorstr( "window that the auto clicker will work in" ) );

			static char buffer_w[ 16 ];

			switch ( config.clicker.version_type )
			{
				case 0:
					config.clicker.window_title = xorstr( "Badlion" );
					break;
				case 1:
					config.clicker.window_title = xorstr( "Lunar" );
					break;
				case 2:
					config.clicker.window_title = xorstr( "Minecraft" );
					break;
				case 3:
					ImGui::InputText( xorstr( "title" ), buffer_w, IM_ARRAYSIZE( buffer_w ) );
					config.clicker.window_title = buffer_w;
					break;
			}

			if ( config.clicker.max_cps <= config.clicker.min_cps && !( config.clicker.min_cps > 19 ) )
				config.clicker.max_cps += 1;

			ImGui::Text( xorstr( "is button down %s" ), var::b_mouse_down ? xorstr( ICON_FA_CHECK " " ) : xorstr( ICON_FA_TIMES " " ) );
			ImGui::Text( xorstr( "clicks on this session %d" ), var::i_clicks_this_session );
			ImGui::Text( xorstr( "application average %.1f ms (%.1f fps)" ), 1000.0f / ImGui::GetIO( ).Framerate, ImGui::GetIO( ).Framerate );

			ImGui::BeginChild( xorstr( "config" ), ImVec2( 280, 155 ), true );
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

					if ( ImGui::InputText( xorstr( "name" ), buffer, IM_ARRAYSIZE( buffer ), ImGuiInputTextFlags_EnterReturnsTrue ) )
					{
						if ( current_config != -1 )
							config.rename( current_config, buffer );
					}

					if ( ImGui::Button( ( xorstr( "create " ICON_FA_PLUS ) ), ImVec2( 75, 20 ) ) )
					{
						config.add( buffer );
					}

					ImGui::SameLine( );

					if ( ImGui::Button( ( xorstr( "reset " ICON_FA_REDO ) ), ImVec2( 75, 20 ) ) )
					{
						config.reset( );
					}

					if ( current_config != -1 )
					{
						if ( ImGui::Button( ( xorstr( "load " ICON_FA_LONG_ARROW_ALT_UP ) ), ImVec2( 75, 20 ) ) )
						{
							config.load( current_config );
						}

						ImGui::SameLine( );

						if ( ImGui::Button( ( xorstr( "save " ICON_FA_SAVE ) ), ImVec2( 75, 20 ) ) )
						{
							config.save( current_config );
						}

						if ( ImGui::Button( ( xorstr( "delete " ICON_FA_TRASH ) ), ImVec2( 75, 20 ) ) )
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

void ui::key_bind_button( int &key, int width, int height )
{
	static auto b_get = false;
	static std::string sz_text = xorstr( "click to bind" );

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
		sz_text = xorstr( "press a key" );
	}
	else if ( !b_get && key == 0 )
		sz_text = xorstr( "click to bind" );
	else if ( !b_get && key != 0 )
		sz_text = xorstr( "bound to " ) + util::string::to_lower( ui::get_key_name_by_id( key ) );
}

void ui::activation_type( )
{
	switch ( config.clicker.activation_type )
	{
		case 0:
			config.clicker.clicker_enabled = true;
			break;

		case 1:
			if ( LI_FN( GetAsyncKeyState ).safe_cached( )( config.clicker.enabled_key ) )
				config.clicker.clicker_enabled = true;
			else
				config.clicker.clicker_enabled = false;
			break;

		case 2:
			if ( LI_FN( GetAsyncKeyState ).safe_cached( )( config.clicker.enabled_key ) )
			{
				var::b_is_clicked = false;
				var::b_is_down = true;
			}
			else if ( !LI_FN( GetAsyncKeyState ).safe_cached( )( config.clicker.enabled_key ) && var::b_is_down )
			{
				var::b_is_clicked = true;
				var::b_is_down = false;
			}
			else
			{
				var::b_is_clicked = false;
				var::b_is_down = false;
			}

			if ( var::b_is_clicked )
				config.clicker.clicker_enabled = !config.clicker.clicker_enabled;

			break;

	}
}

bool ui::create( )
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
		550,
		350,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	if ( !ui::d3d9::create_device_d3d( hwnd ) )
	{
		ui::d3d9::cleanup_device_d3d( );
		LI_FN( UnregisterClassA ).safe_cached( )( wc.lpszClassName, wc.hInstance );

		return false;
	}

	LI_FN( ShowWindow ).safe_cached( )( hwnd, SW_SHOWDEFAULT );
	LI_FN( UpdateWindow ).safe_cached( )( hwnd );

	ImGui::CreateContext( );

	ImGuiStyle *style = &ImGui::GetStyle( );
	ImGuiIO &io = ImGui::GetIO( );

	io.Fonts->AddFontDefault( );

	io.IniFilename = nullptr; // no config file

	ImFontConfig config;
	config.MergeMode = true;
	config.PixelSnapH = true;

	static const ImWchar ranges[] =
	{
		ICON_MIN_FA,
		ICON_MAX_FA,
		0
	};

	io.Fonts->AddFontFromMemoryCompressedTTF( fa_compressed_data, fa_compressed_size, 13.5f, &config, ranges );

	style->WindowRounding = 0.0f;
	style->ChildRounding = 5.0f;
	style->FrameRounding = 4.0f;
	style->GrabRounding = 10.0f;

	// use demo to see color documentation and stuff

	style->Colors[ ImGuiCol_Text ] = color( 255, 255, 255 );
	style->Colors[ ImGuiCol_TextDisabled ] = color( 204, 204, 204 );
	style->Colors[ ImGuiCol_WindowBg ] = color( 25, 25, 25 );
	style->Colors[ ImGuiCol_PopupBg ] = color( 31, 31, 31 );
	style->Colors[ ImGuiCol_Border ] = color( 244, 154, 255 );
	style->Colors[ ImGuiCol_BorderShadow ] = color( 241, 123, 255 );
	style->Colors[ ImGuiCol_FrameBg ] = color( 32, 32, 32 );
	style->Colors[ ImGuiCol_FrameBgHovered ] = color( 51, 51, 51 );
	style->Colors[ ImGuiCol_FrameBgActive ] = color( 74, 74, 74 );
	style->Colors[ ImGuiCol_Button ] = color( 239, 104, 255 );
	style->Colors[ ImGuiCol_ButtonHovered ] = color( 240, 123, 254 );
	style->Colors[ ImGuiCol_ButtonActive ] = color( 240, 142, 252 );
	style->Colors[ ImGuiCol_ScrollbarGrab ] = color( 244, 154, 255 );
	style->Colors[ ImGuiCol_ScrollbarBg ] = color( 25, 25, 25 );
	style->Colors[ ImGuiCol_ScrollbarGrabHovered ] = color( 240, 123, 254 );
	style->Colors[ ImGuiCol_ScrollbarGrabActive ] = color( 240, 142, 252 );
	style->Colors[ ImGuiCol_SliderGrab ] = color( 239, 104, 255 );
	style->Colors[ ImGuiCol_SliderGrabActive ] = color( 240, 142, 252 );
	style->Colors[ ImGuiCol_CheckMark ] = color( 240, 142, 252 );
	style->Colors[ ImGuiCol_Header ] = color( 240, 142, 252 );
	style->Colors[ ImGuiCol_HeaderHovered ] = color( 240, 123, 254 );
	style->Colors[ ImGuiCol_HeaderActive ] = color( 240, 142, 252 );

	if ( !ImGui_ImplWin32_Init( hwnd ) )
		return false;

	if ( !ImGui_ImplDX9_Init( device ) )
		return false;

	ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

	MSG lpMsg;
	ZeroMemory( &lpMsg, sizeof( lpMsg ) );

	_log( LDEBUG, xorstr( "waiting for program end" ) );

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
	ui::dispose( hwnd, wc );

	return true;
}

void ui::dispose( HWND hwnd, WNDCLASSEX wc )
{
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