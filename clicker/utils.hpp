#pragma once

#include <TlHelp32.h>
#include <winioctl.h>

#include "scanner.hpp"

namespace utils
{
	namespace string
	{
		/// <summary>
		/// Converts a string to lower format
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		std::string to_lower( std::string str );

		/// <summary>
		/// Converts a string to wide string
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		std::wstring string_to_wstring( std::string str );

		/// <summary>
		/// Formats a string
		/// </summary>
		/// <typeparam name="...args"></typeparam>
		/// <param name="format"></param>
		/// <param name="...arg"></param>
		/// <returns></returns>
		template<typename ...args>
		static std::string format( const std::string &format, args ...arg );
	}

	namespace floating
	{
		/// <summary>
		/// Random number between start and end using mersenne twister and random_device, returns and receives a float number.
		/// </summary>
		/// <param name="start"></param>
		/// <param name="end"></param>
		/// <returns></returns>
		float random( float start, float end );
	}

	namespace other
	{
		/// <summary>
		/// Returns the active window title
		/// </summary>
		/// <returns></returns>
		std::string get_active_window_title();

		/// <summary>
		/// Returns the process id by name
		/// </summary>
		/// <param name="p_name"></param>
		/// <returns></returns>
		DWORD get_process_id_by_name( const std::string &p_name );

		/// <summary>
		/// Self deletes something
		/// </summary>
		/// <param name="file_path"></param>
		bool self_delete( std::string name, bool is_folder = false );

		/// <summary>
		/// Returns if the current application is focused or not
		/// </summary>
		/// <returns></returns>
		bool application_focused();

		/// <summary>
		/// Returns true when the cursor is visible, used to check if person is in game.
		/// </summary>
		/// <returns></returns>
		bool is_cursor_visible();

		/// <summary>
		/// Clear specified strings on the selected processes
		/// </summary>
		/// <param name="process"></param>
		void clear_strings( std::vector<std::pair<std::string, std::string>> info );

		/// <summary>
		/// Returns disk id
		/// </summary>
		/// <returns></returns>
		std::string get_disk_id();

		/// <summary>
		/// Returns if the user has the window focused
		/// </summary>
		/// <returns></returns>
		bool focused_situation();

		/// <summary>
		/// Used to check if the cursor is visible or not
		/// </summary>
		/// <returns></returns>
		bool get_cursor_status();
	}
}
