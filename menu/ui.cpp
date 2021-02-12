#include "ui.hpp"

void ui::render( )
{
	ImGui::NewFrame( );
	{
		ImGui::Begin( xorstr( "clicker" ), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar );
		{
			ImGui::SetWindowSize( ImVec2( 550, 350 ) );
			ImGui::SetWindowPos( ImVec2( 0, 0 ) );

			ImGui::Text( "Keybindings" );
			ImGui::Separator( );
			{
				ImGui::Combo( xorstr( "##combo" ), &config.clicker.activation_type, xorstr( "Always On\0Hold\0Toggle\0\0" ) );
				ImGui::SameLine( );
				ui::key_bind_button( config.clicker.key, 155, 20 );
				ui::activation_type( );
			}

			ImGui::Text( "Clicker configuration" );
			ImGui::Separator( );
			{
				ImGui::Checkbox( xorstr( "Left clicker enabled##lc_enabled" ), &config.clicker.left_enabled );

				ImGui::SliderInt( xorstr( "##l_maxcps" ), &config.clicker.l_min_cps, 1, 20, xorstr( "Maximum CPS %d" ) );
				ImGui::SliderInt( xorstr( "##l_mincps" ), &config.clicker.l_max_cps, 1, 20, xorstr( "Minimum CPS %d" ) );

				ImGui::Checkbox( xorstr( "Right clicker enabled##lr_enabled" ), &config.clicker.right_enabled );

				ImGui::SliderInt( xorstr( "##r_maxcps" ), &config.clicker.r_min_cps, 1, 20, xorstr( "Maximum CPS %d" ) );
				ImGui::SliderInt( xorstr( "##r_mincps" ), &config.clicker.r_max_cps, 1, 20, xorstr( "Minimum CPS %d" ) );

				ImGui::Combo( xorstr( "Client Version" ), &config.clicker.version_type, xorstr( "Lunar\0Badlion\0Minecraft\0Custom\0\0" ) ); // thanks imgui
				if ( ImGui::IsItemHovered( ) )
					ImGui::SetTooltip( xorstr( "Window that the auto clicker will work in." ) );

				ImGui::Checkbox( xorstr( "Blockhit" ), &config.clicker.blockhit );
				if ( config.clicker.blockhit )
				{
					ImGui::SliderInt( xorstr( "##blockhit_chance" ), &config.clicker.blockhit_chance, 1, 100, xorstr( "Blockhit chance %d%%" ) );
				}
			}

			static char buffer_w[ 16 ];

			switch ( config.clicker.version_type )
			{
				case 0:
					config.clicker.window_title = xorstr( "Lunar" );
					break;
				case 1:
					config.clicker.window_title = xorstr( "Badlion" );
					break;
				case 2:
					config.clicker.window_title = xorstr( "Minecraft" );
					break;
				case 3:
					ImGui::InputText( xorstr( "title" ), buffer_w, IM_ARRAYSIZE( buffer_w ) );
					config.clicker.window_title = buffer_w;
					break;
			}

			if ( config.clicker.l_min_cps <= config.clicker.l_max_cps && !( config.clicker.l_max_cps > 19 ) )
				config.clicker.l_min_cps += 1;

			if ( config.clicker.r_min_cps <= config.clicker.r_max_cps && !( config.clicker.r_max_cps > 19 ) )
				config.clicker.r_min_cps += 1;

			ImGui::Text( "Information" );
			ImGui::Separator( );
			{
				ImGui::Text( xorstr( "Is button down? %s" ), var::b_l_mouse_down ? xorstr( ICON_FA_CHECK " " ) : xorstr( ICON_FA_TIMES " " ) );
				ImGui::Text( xorstr( "Is hotkey toggled? %s" ), config.clicker.hotkey_enabled ? xorstr( ICON_FA_CHECK " " ) : xorstr( ICON_FA_TIMES " " ) );
				ImGui::Text( xorstr( "Clicks this session: %d" ), var::i_clicks_this_session );

#if _DEBUG
				ImGui::Text( xorstr( "Application average %.1f ms (%.1f fps)" ), 1000.0f / ImGui::GetIO( ).Framerate, ImGui::GetIO( ).Framerate );
#endif
			}

			ImGui::Text( "Config settings" );
			ImGui::Separator( );
			{

				ImGui::Checkbox( "Show config settings", &config.clicker.config_show );
				if ( config.clicker.config_show )
				{
					ImGui::BeginChild( xorstr( "config" ), ImVec2( 350, 150 ), false );
					{
						constexpr auto &config_items = config.get_configs( );
						static int current_config = -1;

						if ( static_cast< size_t >( current_config ) >= config_items.size( ) )
							current_config = -1;

						static char buffer[ 16 ];

						if ( ImGui::ListBox( xorstr( "Configs" ), &current_config, [ ]( void *data, int idx, const char **out_text )
							{
								auto &vector = *static_cast< std::vector<std::string> * >( data );
								*out_text = vector[ idx ].c_str( );
								return true;
							}, &config_items, config_items.size( ), 5 ) && current_config != -1 )

							strcpy_s( buffer, config_items[ current_config ].c_str( ) );

							if ( ImGui::InputText( xorstr( "Config name" ), buffer, IM_ARRAYSIZE( buffer ), ImGuiInputTextFlags_EnterReturnsTrue ) )
							{
								if ( current_config != -1 )
									config.rename( current_config, buffer );
							}

							if ( ImGui::Button( ( xorstr( "Create " ICON_FA_PLUS ) ), ImVec2( 75, 20 ) ) )
							{
								config.add( buffer );
							}

							ImGui::SameLine( );

							if ( ImGui::Button( ( xorstr( "Reset " ICON_FA_REDO ) ), ImVec2( 75, 20 ) ) )
							{
								config.reset( );
							}

							if ( current_config != -1 )
							{
								if ( ImGui::Button( ( xorstr( "Load " ICON_FA_LONG_ARROW_ALT_UP ) ), ImVec2( 75, 20 ) ) )
								{
									config.load( current_config );
								}

								ImGui::SameLine( );

								if ( ImGui::Button( ( xorstr( "Save " ICON_FA_SAVE ) ), ImVec2( 75, 20 ) ) )
								{
									config.save( current_config );
								}

								if ( ImGui::Button( ( xorstr( "Delete " ICON_FA_TRASH ) ), ImVec2( 75, 20 ) ) )
								{
									config.remove( current_config );
								}
							}
					}
					ImGui::EndChild( );
				}
			}
		}
		ImGui::End( );
	}
	ImGui::EndFrame( );
}

void ui::key_bind_button( int &key, int width, int height )
{
	static auto b_get = false;
	static std::string sz_text = xorstr( "Click to bind" );

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
		sz_text = xorstr( "Press a key" );
	}
	else if ( !b_get && key == 0 )
		sz_text = xorstr( "Click to bind" );
	else if ( !b_get && key != 0 )
		sz_text = xorstr( "Bound to " ) + util::string::to_lower( ui::get_key_name_by_id( key ) );
}

void ui::activation_type( )
{
	switch ( config.clicker.activation_type )
	{
		case 0:
			if ( config.clicker.left_enabled || config.clicker.right_enabled )
				config.clicker.hotkey_enabled = true;
			break;

		case 1:
			if ( LI_FN( GetAsyncKeyState ).cached( )( config.clicker.key ) )
				config.clicker.hotkey_enabled = true;
			else
				config.clicker.hotkey_enabled = false;
			break;

		case 2:
			if ( LI_FN( GetAsyncKeyState ).cached( )( config.clicker.key ) )
			{
				var::b_is_clicked = false;
				var::b_is_down = true;
			}
			else if ( !LI_FN( GetAsyncKeyState ).cached( )( config.clicker.key ) && var::b_is_down )
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
				config.clicker.hotkey_enabled = !config.clicker.hotkey_enabled;

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
		LI_FN( GetModuleHandleA ).cached( )( nullptr ),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		xorstr( "w_w" ),
		nullptr
	};

	LI_FN( RegisterClassExA ).cached( )( &wc );

	HWND hwnd = CreateWindowA
	(
		wc.lpszClassName,
		xorstr( "" ),
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_POPUP,
		100,
		100,
		566,
		389,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	if ( !ui::d3d9::create_device_d3d( hwnd ) )
	{
		ui::d3d9::cleanup_device_d3d( );
		LI_FN( UnregisterClassA ).cached( )( wc.lpszClassName, wc.hInstance );

		return false;
	}

	LI_FN( ShowWindow ).cached( )( hwnd, SW_SHOWDEFAULT );
	LI_FN( UpdateWindow ).cached( )( hwnd );

	ImGui::CreateContext( );

	ImGuiStyle *style = &ImGui::GetStyle( );
	ImGuiIO &io = ImGui::GetIO( );

	io.Fonts->AddFontDefault( );

	io.IniFilename = nullptr; // No ImGui config file

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

	_log( LDEBUG, xorstr( "Waiting for program end" ) );

	while ( lpMsg.message != WM_QUIT )
	{
		if ( LI_FN( PeekMessageA ).cached( )( &lpMsg, nullptr, 0U, 0U, PM_REMOVE ) )
		{
			LI_FN( TranslateMessage ).cached( )( &lpMsg );
			LI_FN( DispatchMessageA ).cached( )( &lpMsg );
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

	LI_FN( DestroyWindow ).cached( )( hwnd );
	LI_FN( UnregisterClassA ).cached( )( wc.lpszClassName, wc.hInstance );
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
			LI_FN( PostQuitMessage ).cached( )( 0 );
			return 0;
	}

	return LI_FN( DefWindowProcA ).cached( )( hwnd, msg, wParam, lParam );
}