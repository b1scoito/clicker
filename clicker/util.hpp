#pragma once

#include <TlHelp32.h>

namespace util
{
	namespace string
	{
		/// <summary>
		/// String to lower
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		std::string to_lower( std::string str );

		/// <summary>
		/// Converts string to wide string
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

	namespace numbers
	{
		/// <summary>
		/// Random number between start and end using mersenne
		/// </summary>
		/// <param name="start"></param>
		/// <param name="end"></param>
		/// <returns></returns>
		int random( int start, int end );
	}

	namespace other
	{
		/// <summary>
		/// Returns the active window title
		/// </summary>
		/// <returns></returns>
		std::string get_active_window_title( );

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
		void self_delete( std::string path, bool is_folder );

		/// <summary>
		/// Returns if the current application is focused or not
		/// </summary>
		/// <returns></returns>
		bool application_focused( );

		/// <summary>
		/// Returns true when the cursor is visible, used to check if person is in game.
		/// </summary>
		/// <returns></returns>
		bool is_cursor_visible( );
	}
}
