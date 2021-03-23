#include "pch.hpp"
#include "menu.hpp"

void menu::render_objects( HWND hwnd, int width, int height )
{
	static auto mouse_offset_x = 0, mouse_offset_y = 0;

	ImGui::SetNextWindowSize( ImVec2( static_cast<float>(width), static_cast<float>(height) ), ImGuiCond_Always );
	ImGui::SetNextWindowPos( ImVec2( 0, 0 ), ImGuiCond_Always );

	if (ImGui::IsMouseClicked( ImGuiMouseButton_Left ))
		get_mouse_offset( mouse_offset_x, mouse_offset_y, hwnd );

	ImGui::Begin( "##begin", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );
	{
		if (mouse_offset_y >= 0 && mouse_offset_y <= ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f && ImGui::IsMouseDragging( ImGuiMouseButton_Left ))
			set_position( mouse_offset_x, mouse_offset_y, width, height, false, hwnd );

		ImGui::Text( "p-p" );
		ImGui::PushStyleColor( ImGuiCol_Button, color( 255, 255, 255, 0 ) );

		ImGui::SameLine( 0.0f, static_cast<float>(width) - 120.0f );

		// You going to change this? o.o
		if (ImGui::Button( "git" ))
			ShellExecute( 0, 0, "https://github.com/b1scoito/clicker", 0, 0, SW_SHOW );

		ImGui::SameLine();

		if (ImGui::Button( ICON_FA_CLONE ))
			ShowWindow( hwnd, SW_MINIMIZE );

		ImGui::SameLine();

		if (ImGui::Button( ICON_FA_TIMES ))
		{
			// Hiding for the string cleaning operation
			ShowWindow( hwnd, SW_HIDE );

			// This will trigger atexit
			std::exit( 0 );
		}

		ImGui::PopStyleColor();

		if (ImGui::BeginTabBar( "##tabs" ))
		{
			if (ImGui::BeginTabItem( "clicker" ))
			{
				ImGui::Text( "Keybindings" );

				ImGui::Separator();

				g_menu->key_bind_button( config.clicker.key, 155, 22 );

				ImGui::SameLine();

				ImGui::PushItemWidth( 100.f );
				ImGui::Combo( "##cmb_kb_type", &config.clicker.activation_type, "Always on\0Hold\0Toggle\0\0" );
				ImGui::PopItemWidth();


				ImGui::Text( "Clicker configuration" );
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip( "Press Ctrl + Left click on the slider for custom values.\nValues between 9.5-12.5 are recommended for bypassing server-sided anti-cheats." );

				ImGui::Separator();

				ImGui::Checkbox( "Left mouse clicker", &config.clicker.left_enabled );
				if (config.clicker.left_enabled)
					ImGui::SliderFloat( "##l_cps", &config.clicker.left_cps, 1.f, 20.f, "%.1f cps" );

				ImGui::Checkbox( "Right mouse clicker", &config.clicker.right_enabled );
				if (config.clicker.right_enabled)
					ImGui::SliderFloat( "##r_cps", &config.clicker.right_cps, 1.f, 20.f, "%.1f cps" );

				ImGui::Separator();

				ImGui::Checkbox( "Only playing", &config.clicker.only_in_game );
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip( "If enabled, clicker will only work while playing.\nUseful for clicking in game menu." );

				if (config.clicker.only_in_game)
				{
					ImGui::Checkbox( "Work in inventory", &config.clicker.work_in_inventory );
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip( "If enabled, clicker will work while playing,\nwith the inventory opened." );
				}

				ImGui::Checkbox( "Blatant", &config.clicker.blatant );
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip( "If this is checked no randomization will be added.\nUse it at your own risk." );

				if (!config.clicker.blatant)
				{
					ImGui::Checkbox( "Spike chance", &config.clicker.cps_spike_chance );
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip( "Spikes the amount to about 2.5-3.5 cps with the given chance." );

					if (config.clicker.cps_spike_chance)
						ImGui::SliderInt( "##cps_spike_chance", &config.clicker.cps_spike_chance_val, 1, 100, "chance %d%%" );

					ImGui::Checkbox( "Drop chance", &config.clicker.cps_drop_chance );
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip( "Drops the amount to about 2.5-3.5 cps with the given chance." );

					if (config.clicker.cps_drop_chance)
						ImGui::SliderInt( "##cps_drop_chance", &config.clicker.cps_drop_chance_val, 1, 100, "chance %d%%" );

					ImGui::Checkbox( "Blockhit", &config.clicker.blockhit );
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip( "Blockhits automatically with the given chance of blockhit." );

					if (config.clicker.blockhit)
						ImGui::SliderInt( "##blockhit_chance", &config.clicker.blockhit_chance, 1, 100, "chance %d%%" );
				}

				ImGui::Separator();
				ImGui::Combo( "Window", &config.clicker.version_type, "Minecraft\0Custom\0\0" );

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip( "Select custom and leave it blank for it to work anywhere." );

				static char buf[16] {};
				if (config.clicker.version_type == 1)
				{
					ImGui::InputText( "Window Title", buf, IM_ARRAYSIZE( buf ) );
					config.clicker.window_title = buf;
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem( "config" ))
			{
				ImGui::Text( "Config settings" );
				ImGui::Separator();

				if (ImGui::Button( "Open config folder" ))
				{
					PIDLIST_ABSOLUTE pidl;
					if (SUCCEEDED( SHParseDisplayName( util::string::string_to_wstring( config.config_path ).c_str(), 0, &pidl, 0, 0 ) ))
					{
						ITEMIDLIST id_null = { 0 };
						LPCITEMIDLIST pidl_null[1] = { &id_null };
						SHOpenFolderAndSelectItems( pidl, 1, pidl_null, 0 );
						ILFree( pidl );
					}
				}

				constexpr auto &config_items = config.get_configs();
				static int current_config = -1;

				if (static_cast<size_t>(current_config) >= config_items.size())
					current_config = -1;

				static char buffer[16];

				if (ImGui::ListBox( "Configs", &current_config, []( void *data, int idx, const char **out_text )
				{
					auto &vector = *static_cast<std::vector<std::string> *>(data);
					*out_text = vector[idx].c_str();
					return true;
				}, &config_items, config_items.size(), 5 ) && current_config != -1)

					strcpy_s( buffer, config_items[current_config].c_str() );

				if (ImGui::InputText( "Config name", buffer, IM_ARRAYSIZE( buffer ), ImGuiInputTextFlags_EnterReturnsTrue ))
				{
					if (current_config != -1)
						config.rename( current_config, buffer );
				}

				if (ImGui::Button( ("Create"), ImVec2( 60, 25 ) ))
				{
					config.add( buffer );
				}

				ImGui::SameLine();

				if (ImGui::Button( ("Reset"), ImVec2( 60, 25 ) ))
					config.reset();

				ImGui::SameLine();

				if (current_config != -1)
				{
					if (ImGui::Button( ("Load"), ImVec2( 60, 25 ) ))
						config.load( current_config );

					ImGui::SameLine();

					if (ImGui::Button( ("Save"), ImVec2( 60, 25 ) ))
						config.save( current_config );

					ImGui::SameLine();

					if (ImGui::Button( ("Delete"), ImVec2( 60, 25 ) ))
						config.remove( current_config );
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem( "info" ))
			{
				ImGui::Text( "Information" );
				ImGui::Separator();
				ImGui::Text( "Is left button down: %s", var::mouse::left_mouse_down ? ICON_FA_CHECK " " : ICON_FA_TIMES " " );
				ImGui::Text( "Is right button down: %s", var::mouse::right_mouse_down ? ICON_FA_CHECK " " : ICON_FA_TIMES " " );
				ImGui::Text( "Is hotkey toggled: %s", var::key::is_hotkey_enabled ? ICON_FA_CHECK " " : ICON_FA_TIMES " " );
				ImGui::Text( "Clicks this session: %d", var::stats::clicks_this_session );
				ImGui::Text( "Application average: %.1f ms (%.1f fps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem( "self-destruct" ))
			{
				ImGui::Text( "Self-destruct settings" );
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip( "The self-destruct works when you close the program.\nIt will hide itself and exit when the cleaning process finishes.\nYou will hear a beep when it finishes." );
				ImGui::Separator();
				ImGui::Checkbox( "Delete file on exit", &config.clicker.delete_file_on_exit );
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip( "Will self delete the executable on exit." );

				ImGui::Checkbox( "Delete config folder on exit", &config.clicker.delete_config_folder_on_exit );
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip( "Will delete the config folder on exit." );

				ImGui::Checkbox( "Clear strings on exit", &config.clicker.clear_strings_on_exit );
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip( "Will clear strings defined in project on exit.\n(!) It might break your explorer visually. But it's just visually." );

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT WINAPI wndproc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if (ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ))
		return true;

	switch (msg)
	{
		case WM_SIZE:
			if (g_menu->g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
			{
				g_menu->g_d3dpp.BackBufferWidth = LOWORD( lParam );
				g_menu->g_d3dpp.BackBufferHeight = HIWORD( lParam );
				g_menu->reset_device();
			}
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU)
				return 0;
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			return 0;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

bool menu::create( int width, int height )
{
	// https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	WNDCLASSEX wc =
	{
		sizeof( WNDCLASSEX ),
		CS_CLASSDC,
		wndproc,
		0L,
		0L,
		GetModuleHandle( NULL ),
		NULL,
		NULL,
		NULL,
		NULL,
		_T( "w" ),
		NULL
	};

	RegisterClassEx( &wc );

	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		_T( "" ),
		WS_POPUP,
		100,
		100,
		width,
		height,
		NULL,
		NULL,
		wc.hInstance,
		NULL
	);

	if (!g_menu->create_device_d3d( hwnd ))
	{
		g_menu->cleanup_device_d3d();
		UnregisterClass( wc.lpszClassName, wc.hInstance );
		return false;
	}

	ShowWindow( hwnd, SW_SHOWDEFAULT );
	UpdateWindow( hwnd );

	ImGui::CreateContext();

	ImGuiIO &io = ImGui::GetIO(); (void) io;
	ImGuiStyle &style = ImGui::GetStyle(); (void) style;
	ImVec4 *colors = style.Colors;

	io.IniFilename = nullptr;

	static const ImWchar ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig f_config; f_config.MergeMode = true; f_config.PixelSnapH = true;

	if (PWSTR path_to_fonts; SUCCEEDED( SHGetKnownFolderPath( FOLDERID_Fonts, 0, nullptr, &path_to_fonts ) ))
	{
		const std::filesystem::path path { path_to_fonts };
		CoTaskMemFree( path_to_fonts );

		io.Fonts->AddFontFromFileTTF( (path / "tahoma.ttf").string().c_str(), 16.0f, NULL );
	}

	io.Fonts->AddFontFromMemoryCompressedTTF( fa_compressed_data, fa_compressed_size, 13.0f, &f_config, ranges );

	// Hello, if you're a copy man, please take some time to do some fancy things here!

	style.ScrollbarSize = 5.0f;
	style.ChildRounding = 1.0f;
	style.FrameRounding = 3.0f;
	style.GrabRounding = 3.0f;
	style.WindowRounding = 3.0f;

	// Use demo to see color documentation and stuff.
	colors[ImGuiCol_Text] = color( 250, 250, 250 );
	colors[ImGuiCol_TextDisabled] = color( 204, 204, 204 );
	colors[ImGuiCol_WindowBg] = color( 25, 25, 25 );
	colors[ImGuiCol_PopupBg] = color( 31, 31, 31 );
	colors[ImGuiCol_Border] = color( 157, 103, 219 );
	colors[ImGuiCol_BorderShadow] = color( 157, 103, 219 );
	colors[ImGuiCol_FrameBg] = color( 32, 32, 32 );
	colors[ImGuiCol_FrameBgHovered] = color( 51, 51, 51 );
	colors[ImGuiCol_FrameBgActive] = color( 74, 74, 74 );
	colors[ImGuiCol_Button] = color( 157, 103, 219 );
	colors[ImGuiCol_ButtonHovered] = color( 181, 134, 236 );
	colors[ImGuiCol_ButtonActive] = color( 157, 103, 219 );
	colors[ImGuiCol_ScrollbarGrab] = color( 130, 61, 184 );
	colors[ImGuiCol_ScrollbarBg] = color( 25, 25, 25 );
	colors[ImGuiCol_ScrollbarGrabHovered] = color( 157, 103, 219 );
	colors[ImGuiCol_ScrollbarGrabActive] = color( 130, 61, 184 );
	colors[ImGuiCol_SliderGrab] = color( 130, 81, 187 );
	colors[ImGuiCol_SliderGrabActive] = color( 157, 103, 219 );
	colors[ImGuiCol_CheckMark] = color( 130, 81, 187 );
	colors[ImGuiCol_Header] = color( 157, 103, 219 );
	colors[ImGuiCol_HeaderHovered] = color( 157, 103, 219 );
	colors[ImGuiCol_HeaderActive] = color( 157, 103, 219 );
	colors[ImGuiCol_Separator] = color( 239, 104, 255, 150 );
	colors[ImGuiCol_Tab] = color( 157, 103, 219 );
	colors[ImGuiCol_TabHovered] = color( 181, 134, 236 );
	colors[ImGuiCol_TabActive] = color( 157, 103, 219 );

	ImGui_ImplWin32_Init( hwnd );
	ImGui_ImplDX9_Init( g_pd3dDevice );

	ImVec4 clear_color = color( 0, 0, 0, 0 );

	_logd( "Waiting for program end." );

	// fixing issues related to performance and frame capping?
	if (1000.f / ImGui::GetIO().Framerate < 1000.f / 60)
		std::this_thread::sleep_for( std::chrono::milliseconds( (long long) (1000.f / 60) ) );

	MSG msg {};
	ZeroMemory( &msg, sizeof( msg ) );
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			continue;
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		// Render our objects
		g_menu->render_objects( hwnd, width, height );

		ImGui::EndFrame();

		g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
		g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		g_pd3dDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );

		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA
		(
			(int) (clear_color.x * 255.0f),
			(int) (clear_color.y * 255.0f),
			(int) (clear_color.z * 255.0f),
			(int) (clear_color.w * 255.0f)
		);

		g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );

		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );
			g_pd3dDevice->EndScene();
		}

		HRESULT result = g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			g_menu->reset_device();
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	g_menu->cleanup_device_d3d();
	DestroyWindow( hwnd );
	UnregisterClass( wc.lpszClassName, wc.hInstance );

	return true;
}