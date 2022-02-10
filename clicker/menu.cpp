#include "pch.hpp"
#include "menu.hpp"

void c_menu::on_paint( HWND hwnd, int i_width, int i_height )
{
	static int x = 0, y = 0;

	ImGui::SetNextWindowSize( { (float)i_width, (float)i_height }, ImGuiCond_Once );
	ImGui::SetNextWindowPos( { 0, 0 }, ImGuiCond_Once );

	vars::key::hide_window.i_key = config.clicker.i_hide_window_key;
	vars::key::hide_window.get() ? ShowWindow( hwnd, SW_HIDE ) : ShowWindow( hwnd, SW_SHOW );

	if ( !vars::b_is_running )
		std::exit( 0 );

	if ( ImGui::Begin( "clicker", &vars::b_is_running, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove ) )
	{
		if ( ImGui::IsMouseClicked( ImGuiMouseButton_Left ) )
			this->get_mouse_offset( x, y, hwnd );

		if ( y >= 0 && y <= ( ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 4 ) && ImGui::IsMouseDragging( ImGuiMouseButton_Left ) )
			this->set_position( x, y, i_width, i_height, hwnd );

		if ( ImGui::BeginTabBar( "##var::clicker::tabs" ) )
		{
			if ( ImGui::BeginTabItem( "Mouse" ) )
			{
				ImGui::Text( "Keybind" );
				ImGui::Separator();

				this->keybind_button( config.clicker.i_clicker_key, 150, 22 );

				ImGui::SameLine();

				ImGui::PushItemWidth( 100.f );
				ImGui::Combo( "##var::clicker::i_key_type", &config.clicker.i_key_type, "Always\0Hold\0Toggle\0\0" );
				ImGui::PopItemWidth();

				ImGui::Text( "General" );
				if ( ImGui::IsItemHovered() )
					ImGui::SetTooltip( "Press Ctrl + Left click on the slider for custom values.\nValues between 9.5f - 12.5f are recommended for bypassing server-sided anti-cheats." );

				ImGui::Separator();

				ImGui::Checkbox( "Left mouse clicker", &config.clicker.b_enable_left_clicker );
				if ( config.clicker.b_enable_left_clicker )
					ImGui::SliderFloat( "##var::clicker::f_left_cps", &config.clicker.f_left_cps, 1.f, 20.f, "%.2f cps" );

				ImGui::Checkbox( "Right mouse clicker", &config.clicker.b_enable_right_clicker );
				if ( config.clicker.b_enable_right_clicker )
					ImGui::SliderFloat( "##var::clicker::f_right_cps", &config.clicker.f_right_cps, 1.f, 20.f, "%.2f cps" );

				ImGui::Text( "Focus" );
				ImGui::Separator();

				ImGui::Combo( "##var::clicker::i_version_type", &config.clicker.i_version_type, "Minecraft\0Custom\0\0" );

				switch ( config.clicker.i_version_type )
				{
					case 0:
							//ImGui::Checkbox( "Work in inventory", &config.clicker.b_work_in_inventory );
							//if ( ImGui::IsItemHovered() )
							//	ImGui::SetTooltip( "If enabled, the clicker will work with the inventory opened." );
						break;
					case 1:
						static char window_name_buffer[32];
						ImGui::InputText( "##var::input::buffer", window_name_buffer, IM_ARRAYSIZE( window_name_buffer ) );
						if ( ImGui::IsItemHovered() )
							ImGui::SetTooltip( "If you leave it blank it'll work anywhere." );

						config.clicker.str_window_title = window_name_buffer;
						break;
				}

				if ( !config.clicker.b_enable_blatant )
				{
					ImGui::Text( "Randomization" );
					ImGui::Separator();

					ImGui::Checkbox( "Persistence", &config.clicker.b_enable_persistence );
					if ( ImGui::IsItemHovered() )
						ImGui::SetTooltip( "This randomization is applied in a short period of time set to the defined CPS.\nRecommended." );

					if ( config.clicker.b_enable_persistence )
						ImGui::SliderFloat( "##var::clicker::f_persistence_value", &config.clicker.f_persistence_value, 1.f, 5.f, "%.1f cps" );

					ImGui::Checkbox( "Spike chance", &config.clicker.b_enable_cps_spikes );
					if ( ImGui::IsItemHovered() )
						ImGui::SetTooltip( "Spikes the amount to defined value with the given chance." );

					if ( config.clicker.b_enable_cps_spikes )
					{
						ImGui::SliderInt( "##var::clicker::i_cps_spike_chance", &config.clicker.i_cps_spike_chance, 1, 100, "chance %d%%" );
						ImGui::SliderFloat( "##var::clicker::f_cps_spike_add", &config.clicker.f_cps_spike_add, 1.f, 5.f, "add %.1f cps" );
					}

					ImGui::Checkbox( "Drop chance", &config.clicker.b_enable_cps_drops );
					if ( ImGui::IsItemHovered() )
						ImGui::SetTooltip( "Drops the amount to defined value with the given chance." );

					if ( config.clicker.b_enable_cps_drops )
					{
						ImGui::SliderInt( "##var::clicker::i_cps_drop_chance", &config.clicker.i_cps_drop_chance, 1, 100, "chance %d%%" );
						ImGui::SliderFloat( "##var::clicker::f_cps_drop_remove", &config.clicker.f_cps_drop_remove, 1.f, 5.f, "remove %.1f cps" );
					}

					ImGui::Checkbox( "Blockhit", &config.clicker.b_enable_blockhit );
					if ( ImGui::IsItemHovered() )
						ImGui::SetTooltip( "Blockhits automatically with the given chance of blockhit." );

					if ( config.clicker.b_enable_blockhit )
						ImGui::SliderInt( "##var::clicker::i_blockhit_chance", &config.clicker.i_blockhit_chance, 1, 100, "chance %d%%" );

					ImGui::Checkbox( "Advanced options", &config.clicker.b_enable_advanced_options );
					if ( config.clicker.b_enable_advanced_options )
					{
						ImGui::Text( "Maximum update rate delay" );
						if ( ImGui::IsItemHovered() )
							ImGui::SetTooltip( "Maximum CPS rate delay update." );

						ImGui::SliderFloat( "##var::clicker::f_persistence_update_rate", &config.clicker.f_persistence_update_rate, 500.f, 10000.f, "%.1f ms" );

						ImGui::Text( "Default timer randomization" );
						if ( ImGui::IsItemHovered() )
							ImGui::SetTooltip( "Default timer delay variation." );

						ImGui::SliderFloat( "##var::clicker::f_default_timer_randomization", &config.clicker.f_default_timer_randomization, 1.f, 15.f, "%.1f ms" );
					}
				}

				ImGui::Separator();
				ImGui::Checkbox( "Blatant", &config.clicker.b_enable_blatant );
				if ( ImGui::IsItemHovered() )
					ImGui::SetTooltip( "If this is enabled no randomization will be added. Use it at your own risk." );

				ImGui::EndTabItem();
			}

			if ( ImGui::BeginTabItem( "Misc" ) )
			{
				ImGui::Text( "Hide" );
				ImGui::Separator();
				ImGui::Text( "Hide window key" );
				keybind_button( config.clicker.i_hide_window_key, 155, 22 );

				ImGui::Text("Input");
				ImGui::Separator();
				ImGui::Text("Send input method");
				ImGui::PushItemWidth(200.f);
				ImGui::Combo("##var::clicker::i_send_input_method", &config.clicker.i_send_input_method, "SendMessage\0PostMessage\0\0");
				ImGui::PopItemWidth();
				

				ImGui::Separator();
				ImGui::Text( "Colors" );
				ImGui::Separator();
				ImGui::ColorEdit4( "Color accent", config.clicker.f_color_accent, ImGuiColorEditFlags_NoInputs );
				ImGui::ColorEdit4( "Color accent hovered", config.clicker.f_color_accent_hovered, ImGuiColorEditFlags_NoInputs );
				ImGui::ColorEdit4( "Color accent active", config.clicker.f_color_accent_active, ImGuiColorEditFlags_NoInputs );
				ImGui::ColorEdit4( "Color accent text", config.clicker.f_color_accent_text, ImGuiColorEditFlags_NoInputs );

				ImGui::Separator();
				static bool show_advanced_debug { false };
				ImGui::Checkbox( "Show advanced debug info", &show_advanced_debug );

				if ( show_advanced_debug )
				{
					ImGui::Text( "Information" );
					ImGui::Separator();
					ImGui::Text( "Clicks this session: %d", vars::stats::i_clicks_this_session );
					ImGui::Text( "Average CPS: %.2f", vars::stats::f_average_cps );
					ImGui::Text( "Is left button down: %s", vars::key::is_left_down.b_state ? ICON_FA_CHECK : ICON_FA_TIMES);
					ImGui::Text( "Is right button down: %s", vars::key::is_right_down.b_state ? ICON_FA_CHECK : ICON_FA_TIMES );
					ImGui::Text( "Is hotkey toggled: %s", vars::key::clicker_enabled.get() ? ICON_FA_CHECK : ICON_FA_TIMES );
					ImGui::Text( "Is window focused: %s", focus::window_think() ? ICON_FA_CHECK : ICON_FA_TIMES );
					ImGui::Text( "Is cursor visible: %s", focus::is_cursor_visible() ? ICON_FA_CHECK : ICON_FA_TIMES );
					//ImGui::Text( "Is in inventory: %s", vars::key::b_inventory_opened ? ICON_FA_CHECK : ICON_FA_TIMES );

					if ( !focus::active_window_title().empty() )
						ImGui::Text( "Current window name: %ls", focus::active_window_title().data() );

					ImGui::Text( "Application average: %.1f ms (%.1f fps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
				}

				ImGui::Separator();
				ImGui::Text( "https://github.com/b1scoito/clicker" );
				if ( ImGui::IsItemHovered() )
					ImGui::SetTooltip( "Click me!" );
				if ( ImGui::IsItemClicked( ImGuiMouseButton_Left ) )
					ShellExecute( 0, 0, L"https://github.com/b1scoito/clicker", 0, 0, SW_SHOW );

				ImGui::EndTabItem();
			}

			if ( ImGui::BeginTabItem( "Config" ) )
			{
				ImGui::Text( "Config settings" );
				ImGui::Separator();

				if ( ImGui::Button( "Open config folder" ) )
				{
					if ( PIDLIST_ABSOLUTE pidl; SUCCEEDED( SHParseDisplayName( config.get_path().wstring().data(), 0, &pidl, 0, 0 ) ) )
					{
						ITEMIDLIST id_null = { 0 };
						LPCITEMIDLIST pidl_null[1] = { &id_null };
						SHOpenFolderAndSelectItems( pidl, 1, pidl_null, 0 );
						ILFree( pidl );
					}
				}

				constexpr auto& config_items = config.get_configs();
				static auto current_config = -1;

				if ( (size_t) ( current_config ) >= config_items.size() )
					current_config = -1;

				static char buffer[32];

				ImGui::Text( "Configs" );
				if ( ImGui::ListBox( "##var::clicker::config_list", &current_config, []( void* data, int idx, const char** out_text )
				{
					auto& vector = *( std::vector<std::string> * )( data );
					*out_text = vector[idx].c_str();
					return true;
				}, &config_items, (int) ( config_items.size() ), 5 ) && current_config != -1 ) strcpy_s( buffer, config_items[current_config].c_str() );

				if ( ImGui::InputText( "##var::clicker::config_name", buffer, IM_ARRAYSIZE( buffer ), ImGuiInputTextFlags_EnterReturnsTrue ) )
				{
					if ( current_config != -1 )
						config.rename( current_config, buffer );
				}

				if ( ImGui::Button( "Create", ImVec2( 60, 25 ) ) )
					config.add( buffer );

				ImGui::SameLine();

				if ( ImGui::Button( "Reset", ImVec2( 60, 25 ) ) )
					config.reset();

				ImGui::SameLine();

				if ( current_config > -1 )
				{
					if ( ImGui::Button( "Save", ImVec2( 60, 25 ) ) )
						config.save( current_config );

					ImGui::SameLine();

					if ( ImGui::Button( "Load", ImVec2( 60, 25 ) ) )
						config.load( current_config );

					ImGui::SameLine();

					if ( ImGui::Button( "Delete", ImVec2( 60, 25 ) ) )
						config.remove( current_config );
				}

				ImGui::EndTabItem();
			}
			
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}

void c_menu::keybind_button( int& i_key, int i_width, int i_height )
{
	static auto b_get = false;
	static std::string sz_text( "Click to bind" );

	if ( ImGui::Button( sz_text.c_str(), ImVec2( (float) ( i_width ), (float) ( i_height ) ) ) )
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

std::string c_menu::get_key_name_by_id( int id )
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

void c_menu::set_position( int x, int y, int cx, int cy, HWND hwnd )
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

void c_menu::get_mouse_offset( int& x, int& y, HWND hwnd )
{
	POINT point; RECT rect;

	GetCursorPos( &point );
	GetWindowRect( hwnd, &rect );

	x = point.x - rect.left;
	y = point.y - rect.top;
}

LRESULT WINAPI c_menu::wndproc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
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

bool c_menu::create_device_d3d( HWND hWnd )
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

void c_menu::cleanup_device_d3d()
{
	if ( g_pd3dDevice ) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if ( g_pD3D ) { g_pD3D->Release(); g_pD3D = NULL; }
}

void c_menu::reset_device()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT hr = g_pd3dDevice->Reset( &g_d3dpp );
	if ( hr == D3DERR_INVALIDCALL )
		IM_ASSERT( 0 );

	ImGui_ImplDX9_CreateDeviceObjects();
}

bool c_menu::init( int width, int height ) noexcept
{
	const WNDCLASSEX wc = {
		sizeof( WNDCLASSEX ), CS_CLASSDC,
		this->wndproc,
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

	io.Fonts->AddFontDefault();

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