#include "pch.hpp"
#include "menu.hpp"

void menu::render_items( HWND hwnd, int width, int height )
{
	/* ~~ Defining variables for the mouse offset */
	static auto mouse_offset_x = 0, mouse_offset_y = 0;

	/* ~~ Setting the window size depending on what we defined */
	ImGui::SetNextWindowSize( ImVec2( static_cast<float>(width), static_cast<float>(height) ), ImGuiCond_Always );
	ImGui::SetNextWindowPos( ImVec2( 0, 0 ), ImGuiCond_Always );

	/* ~~ Only gets the mouse offset if person has left-clicked */
	if (ImGui::IsMouseClicked( ImGuiMouseButton_Left ))
		get_mouse_offset( mouse_offset_x, mouse_offset_y, hwnd );

	/* ~~ Our hide window bind floating around this function */
	hooks::keybinds::hide_window.i_key = config.clicker.hide_window_key;
	hooks::keybinds::hide_window.get() ? ShowWindow( hwnd, SW_HIDE ) : ShowWindow( hwnd, SW_SHOW );

	/* ~~ Begins an ImGui Window with no titlebar, no resizing and no moving */
	ImGui::Begin( "##begin", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );
	{
		/* ~~ Where the window dragging happens */
		if (mouse_offset_y >= 0 && mouse_offset_y <= ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 5.f && ImGui::IsMouseDragging( ImGuiMouseButton_Left ))
			set_position( mouse_offset_x, mouse_offset_y, width, height, false, hwnd );

		ImGui::Text( "clicker" );
		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( ImColor( 255, 255, 255, 0 ) ) );

		ImGui::SameLine( 0.f, static_cast<float>(width) - 105.f );

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

				keybind_button( config.clicker.clicker_key, 155, 22 );

				ImGui::SameLine();

				ImGui::PushItemWidth( 100.f );
				ImGui::Combo( "##activation_mode", &config.clicker.activation_type, "Always on\0Hold\0Toggle\0\0" );
				ImGui::PopItemWidth();

				ImGui::Text( "Clicker configuration" );
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip( "Press Ctrl + Left click on the slider for custom values.\nValues between 9.5f - 12.5f are recommended for bypassing server-sided anti-cheats." );

				ImGui::Separator();

				ImGui::Checkbox( "Left mouse clicker", &config.clicker.left_clicker_enabled );
				if (config.clicker.left_clicker_enabled)
					ImGui::SliderFloat( "##left_cps", &config.clicker.left_cps, 1.f, 20.f, "%.2f cps" );

				ImGui::Checkbox( "Right mouse clicker", &config.clicker.right_clicker_enabled );
				if (config.clicker.right_clicker_enabled)
					ImGui::SliderFloat( "##right_cps", &config.clicker.right_cps, 1.f, 20.f, "%.2f cps" );

				ImGui::Text( "Focus settings" );
				ImGui::Separator();

				if (config.clicker.version_type == 0)
				{
					ImGui::Checkbox( "Only playing", &config.clicker.only_in_game );
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip( "If enabled, clicker will only work while playing.\nUseful for clicking in game menu." );

					if (config.clicker.only_in_game)
					{
						ImGui::Checkbox( "Work in inventory", &config.clicker.work_in_inventory );
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip( "If enabled, clicker will work while playing,\nwith the inventory opened." );
					}
				}

				ImGui::Combo( "##window_type", &config.clicker.version_type, "Minecraft\0Custom\0\0" );

				static char buffer[32] {};
				if (config.clicker.version_type == 1)
				{
					ImGui::InputText( "##window_title", buffer, IM_ARRAYSIZE( buffer ) );
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip( "Leave it blank for it to work anywhere." );

					config.clicker.window_title = buffer;
				}

				if (!config.clicker.blatant_enabled)
				{
					ImGui::Text( "Randomization" );
					ImGui::Separator();

					ImGui::Checkbox( "Persistent randomization cps values", &config.clicker.persistent_values_enabled );
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip( "This randomization is applied in a short period of time,\nset to the defined cps randomized.\nRecommended." );

					if (config.clicker.persistent_values_enabled)
						ImGui::SliderFloat( "##default_persistent_randomization", &config.clicker.default_persistent_randomization, 1.f, 5.f, "%.1f cps" );

					ImGui::Checkbox( "Spike chance", &config.clicker.cps_spike_chance );
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip( "Spikes the amount to defined value with the given chance." );

					if (config.clicker.cps_spike_chance)
					{
						ImGui::SliderInt( "##cps_spike_chance", &config.clicker.cps_spike_chance_val, 1, 100, "chance %d%%" );
						ImGui::SliderFloat( "##cps_spike_chance_addition", &config.clicker.cps_spike_chance_value_addition, 1.f, 5.f, "value addition %.1f cps" );
					}

					ImGui::Checkbox( "Drop chance", &config.clicker.cps_drop_chance );
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip( "Drops the amount to defined value with the given chance." );

					if (config.clicker.cps_drop_chance)
					{
						ImGui::SliderInt( "##cps_drop_chance", &config.clicker.cps_drop_chance_val, 1, 100, "chance %d%%" );
						ImGui::SliderFloat( "##cps_drop_chance_removal", &config.clicker.cps_drop_chance_value_removal, 1.f, 5.f, "value removal %.1f cps" );
					}

					ImGui::Checkbox( "Blockhit", &config.clicker.blockhit_enabled );
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip( "Blockhits automatically with the given chance of blockhit." );

					if (config.clicker.blockhit_enabled)
						ImGui::SliderInt( "##blockhit_chance", &config.clicker.blockhit_chance, 1, 100, "chance %d%%" );
				}

				ImGui::Separator();

				ImGui::Checkbox( "Blatant", &config.clicker.blatant_enabled );
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip( "If this is checked no randomization will be added.\nUse it at your own risk." );

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem( "other" ))
			{
				ImGui::Text( "Colors" );
				ImGui::Separator();
				ImGui::ColorEdit4( "Color accent", config.clicker.color_accent );
				ImGui::ColorEdit4( "Color accent hovered", config.clicker.color_accent_hovered );
				ImGui::ColorEdit4( "Color accent active", config.clicker.color_accent_active );
				ImGui::ColorEdit4( "Color accent text", config.clicker.color_accent_text );

				ImGui::Text( "Information" );
				ImGui::Separator();
				ImGui::Text( "Is left button down: %s", vars::mouse::left_mouse_down ? ICON_FA_CHECK " " : ICON_FA_TIMES " " );
				ImGui::Text( "Is right button down: %s", vars::mouse::right_mouse_down ? ICON_FA_CHECK " " : ICON_FA_TIMES " " );
				ImGui::Text( "Is hotkey toggled: %s", vars::key::is_hotkey_enabled ? ICON_FA_CHECK " " : ICON_FA_TIMES " " );
				ImGui::Text( "Is window focused: %s", vars::window::is_focused ? ICON_FA_CHECK " " : ICON_FA_TIMES " " );
				ImGui::Text( "Is cursor visible: %s", vars::window::is_cursor_visible ? ICON_FA_CHECK " " : ICON_FA_TIMES " " );
				ImGui::Text( "Clicks this session: %d", vars::stats::clicks_this_session );
				ImGui::Text( "Application average: %.1f ms (%.1f fps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );

				ImGui::Separator();
				ImGui::Text( "Github repository, hover me!" );
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip( "https://github.com/b1scoito/clicker" );

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem( "config" ))
			{
				ImGui::Text( "Config settings" );
				ImGui::Separator();

				if (ImGui::Button( "Open config folder" ))
				{
					if (PIDLIST_ABSOLUTE pidl; SUCCEEDED( SHParseDisplayName( utils::string::string_to_wstring( config.config_path ).c_str(), 0, &pidl, 0, 0 ) ))
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
				}, &config_items, static_cast<int>(config_items.size()), 5 ) && current_config != -1) strcpy_s( buffer, config_items[current_config].c_str() );

				if (ImGui::InputText( "Config name", buffer, IM_ARRAYSIZE( buffer ), ImGuiInputTextFlags_EnterReturnsTrue ))
				{
					if (current_config != -1)
						config.rename( current_config, buffer );
				}

				if (ImGui::Button( "Create", ImVec2( 60, 25 ) ))
					config.add( buffer );

				ImGui::SameLine();

				if (ImGui::Button( "Reset", ImVec2( 60, 25 ) ))
					config.reset();

				ImGui::SameLine();

				if (current_config != -1)
				{
					if (ImGui::Button( "Load", ImVec2( 60, 25 ) ))
						config.load( current_config );

					ImGui::SameLine();

					if (ImGui::Button( "Save", ImVec2( 60, 25 ) ))
						config.save( current_config );

					ImGui::SameLine();

					if (ImGui::Button( "Delete", ImVec2( 60, 25 ) ))
						config.remove( current_config );
				}
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
					ImGui::SetTooltip( "Will clear strings defined in project on exit.\n(!) It might break your explorer visually, but it's just visually." );

				ImGui::Text( "Hide window key" );
				keybind_button( config.clicker.hide_window_key, 155, 22 );

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}