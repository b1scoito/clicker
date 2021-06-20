#include "pch.hpp"
#include "menu.hpp"

void c_menu::on_paint( HWND hwnd, int i_width, int i_height )
{
	static int x = 0, y = 0;

	ImGui::SetNextWindowSize( { static_cast<float>( i_width ), static_cast<float>( i_height ) }, ImGuiCond_Always );
	ImGui::SetNextWindowPos( { 0, 0 }, ImGuiCond_Always );

	vars::key::hide_window.i_key = config.clicker.i_hide_window_key;
	vars::key::hide_window.get() ? ShowWindow( hwnd, SW_HIDE ) : ShowWindow( hwnd, SW_SHOW );

	static auto b_open = true;

	if ( !b_open )
		std::exit( 0 );

	if ( ImGui::Begin( "clicker", &b_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove ) )
	{
		if ( ImGui::IsMouseClicked( ImGuiMouseButton_Left ) )
			this->get_mouse_offset( x, y, hwnd );

		if ( y >= 0 && y <= ( ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 4 ) && ImGui::IsMouseDragging( ImGuiMouseButton_Left ) )
			this->set_position( x, y, i_width, i_height, hwnd );

		if ( ImGui::BeginTabBar( "##var::clicker::tabs" ) )
		{
			if ( ImGui::BeginTabItem( "mouse" ) )
			{
				ImGui::Text( "Keybind" );
				ImGui::Separator();

				this->keybind_button( config.clicker.i_clicker_key, 150, 22 );

				ImGui::SameLine();

				ImGui::PushItemWidth( 100.f );
				ImGui::Combo( "##var::clicker::i_key_type", &config.clicker.i_key_type, "Always\0Hold\0Toggle\0\0" );
				ImGui::PopItemWidth();

				ImGui::Text( "Clicker configuration" );
				if ( ImGui::IsItemHovered() )
					ImGui::SetTooltip( "Press Ctrl + Left click on the slider for custom values.\nValues between 9.5f - 12.5f are recommended for bypassing server-sided anti-cheats." );

				ImGui::Separator();

				ImGui::Checkbox( "Left mouse clicker", &config.clicker.b_enable_left_clicker );
				if ( config.clicker.b_enable_left_clicker )
					ImGui::SliderFloat( "##var::clicker::f_left_cps", &config.clicker.f_left_cps, 1.f, 20.f, "%.2f cps" );

				ImGui::Checkbox( "Right mouse clicker", &config.clicker.b_enable_right_clicker );
				if ( config.clicker.b_enable_right_clicker )
					ImGui::SliderFloat( "##var::clicker::f_right_cps", &config.clicker.f_right_cps, 1.f, 20.f, "%.2f cps" );

				ImGui::Text( "Focus settings" );
				ImGui::Separator();

				ImGui::Combo( "##var::clicker::i_version_type", &config.clicker.i_version_type, "Minecraft\0Custom\0\0" );

				switch ( config.clicker.i_version_type )
				{
					case 0:
						ImGui::Checkbox( "Only playing", &config.clicker.b_only_in_game );
						if ( ImGui::IsItemHovered() )
							ImGui::SetTooltip( "If enabled, clicker will only work while playing.\nUseful for clicking in game menu." );

						if ( config.clicker.b_only_in_game )
						{
							ImGui::Checkbox( "Work in inventory", &config.clicker.b_work_in_inventory );
							if ( ImGui::IsItemHovered() )
								ImGui::SetTooltip( "If enabled, clicker will work while playing and with the inventory opened." );
						}
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
						ImGui::SetTooltip( "This randomization is applied in a short period of time,\nset to the defined cps randomized.\nRecommended." );

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
							ImGui::SetTooltip( "Smaller values, faster cps updates." );

						ImGui::SliderFloat( "##var::clicker::f_persistence_update_rate", &config.clicker.f_persistence_update_rate, 500.f, 10000.f, "%.1f ms" );

						ImGui::Text( "Default timer randomization" );
						if ( ImGui::IsItemHovered() )
							ImGui::SetTooltip( "Timer time delay variation. Much fancy words" );

						ImGui::SliderFloat( "##var::clicker::f_default_timer_randomization", &config.clicker.f_default_timer_randomization, 1.f, 15.f, "%.1f ms" );
					}
				}

				ImGui::Separator();
				ImGui::Checkbox( "Blatant", &config.clicker.b_enable_blatant );
				if ( ImGui::IsItemHovered() )
					ImGui::SetTooltip( "If this is checked no randomization will be added.\nUse it at your own risk." );

				ImGui::EndTabItem();
			}

			if ( ImGui::BeginTabItem( "misc" ) )
			{
				ImGui::Text( "Hide" );
				ImGui::Separator();
				ImGui::Text( "Hide window key" );
				keybind_button( config.clicker.i_hide_window_key, 155, 22 );

				ImGui::Text( "Colors" );
				ImGui::Separator();
				ImGui::ColorEdit4( "Color accent", config.clicker.f_color_accent, ImGuiColorEditFlags_NoInputs );
				ImGui::ColorEdit4( "Color accent hovered", config.clicker.f_color_accent_hovered, ImGuiColorEditFlags_NoInputs );
				ImGui::ColorEdit4( "Color accent active", config.clicker.f_color_accent_active, ImGuiColorEditFlags_NoInputs );
				ImGui::ColorEdit4( "Color accent text", config.clicker.f_color_accent_text, ImGuiColorEditFlags_NoInputs );

				static bool show_advanced_debug { false };
				ImGui::Checkbox( "Show advanced debug info", &show_advanced_debug );

				if ( show_advanced_debug )
				{
					ImGui::Text( "Information" );
					ImGui::Separator();
					ImGui::Text( "Clicks this session: %d", vars::stats::i_clicks_this_session );
					ImGui::Text( "Average CPS: %.2f", vars::stats::f_average_cps );
					ImGui::Text( "Is left button down: %s", vars::key::left_clicker_down.get() ? ICON_FA_CHECK : ICON_FA_TIMES );
					ImGui::Text( "Is right button down: %s", vars::key::right_clicker_down.get() ? ICON_FA_CHECK : ICON_FA_TIMES );
					ImGui::Text( "Is hotkey toggled: %s", vars::key::clicker_enabled.get() ? ICON_FA_CHECK : ICON_FA_TIMES );
					ImGui::Text( "Is window focused: %s", util::extra::is_window_focused() ? ICON_FA_CHECK : ICON_FA_TIMES );
					ImGui::Text( "Is cursor visible: %s", util::extra::cursor_visible() ? ICON_FA_CHECK : ICON_FA_TIMES );
					ImGui::Text( "Is in inventory: %s", vars::key::inventory_opened ? ICON_FA_CHECK : ICON_FA_TIMES );
					ImGui::Text( "Current window name: %ls", util::extra::get_active_window_title().c_str() );
					ImGui::Text( "Application average: %.1f ms (%.1f fps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
					ImGui::Separator();
					ImGui::Text( "Repository: https://github.com/b1scoito/clicker" );
				}

				ImGui::EndTabItem();
			}

			if ( ImGui::BeginTabItem( "config" ) )
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

				if ( static_cast<size_t>( current_config ) >= config_items.size() )
					current_config = -1;

				static char buffer[32];

				ImGui::Text( "Configs" );
				if ( ImGui::ListBox( "##var::clicker::config_list", &current_config, []( void* data, int idx, const char** out_text )
				{
					auto& vector = *static_cast<std::vector<std::string> *>( data );
					*out_text = vector[idx].c_str();
					return true;
				}, &config_items, static_cast<int>( config_items.size() ), 5 ) && current_config != -1 ) strcpy_s( buffer, config_items[current_config].c_str() );

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
					if ( ImGui::Button( "Load", ImVec2( 60, 25 ) ) )
						config.load( current_config );

					ImGui::SameLine();

					if ( ImGui::Button( "Save", ImVec2( 60, 25 ) ) )
						config.save( current_config );

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