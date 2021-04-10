#include "pch.hpp"
#include "utils.hpp"

std::string utils::string::to_lower( std::string string )
{
	std::transform( string.begin(), string.end(), string.begin(), static_cast<int(*)(int)>(::tolower) );
	return string;
}

std::wstring utils::string::string_to_wstring( std::string string )
{
	if (string.empty())
		return {};

	const auto len = string.length() + 1;
	auto ret = std::wstring( len, 0 );
	const auto size = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, &string[0], static_cast<int>(string.size()), &ret[0], static_cast<int>(len) );
	ret.resize( size );

	return ret;
}

template<typename ... args>
static std::string utils::string::format( const std::string &format, args ... arg )
{
	const size_t size = std::snprintf( nullptr, 0, format.c_str(), arg ... ) + static_cast<size_t>(1);
	std::unique_ptr<char[]> buf( new char[size] );
	std::snprintf( buf.get(), size, format.c_str(), arg ... );
	return std::string( buf.get(), buf.get() + size - 1 );
}

float utils::floating::random( float start, float end )
{
	// https://github.com/EternalRift/clicker/blob/main/source/utils/utils.hpp#L55
	// mersenne twister engine with a random seed based on the clock (once at system startup) 
	static std::mt19937 mersenne { static_cast<std::mt19937::result_type>(std::time( nullptr )) };
	const std::uniform_real_distribution<float> distribution( start, end );
	return distribution( mersenne );
}

std::string utils::other::get_active_window_title()
{
	char title[256] {};
	HWND hwnd = GetForegroundWindow();
	GetWindowText( hwnd, title, sizeof( title ) );
	return title;
}

DWORD utils::other::get_process_id_by_name( const std::string &p_name )
{
	if (p_name.empty())
		return {};

	auto str_fl = p_name;
	if (str_fl.find_last_of( "." ) != std::string::npos)
		str_fl.erase( str_fl.find_last_of( "." ), std::string::npos );

	str_fl += ".exe";

	const auto handle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
	PROCESSENTRY32 m_entry {}; m_entry.dwSize = sizeof( m_entry );

	if (!Process32First( handle, &m_entry ))
		return {};

	while (Process32Next( handle, &m_entry ))
	{
		if (utils::string::to_lower( m_entry.szExeFile ).compare( utils::string::to_lower( str_fl ) ) == 0)
		{
			CloseHandle( handle );
			return m_entry.th32ProcessID;
		}
	}

	return {};
}

bool utils::other::self_delete( std::string name, bool is_folder )
{
	STARTUPINFO si {};
	{
		ZeroMemory( &si, sizeof( si ) );
		si.cb = sizeof( si );
	}

	PROCESS_INFORMATION pi {};
	{
		ZeroMemory( &pi, sizeof( pi ) );
	}

	if (is_folder)
		std::filesystem::remove_all( name );

	if (!CreateProcess(
		nullptr,
		const_cast<LPSTR>(utils::string::format( "cmd.exe /C timeout /t 3 > nul & del /f /q %s", name.c_str() ).c_str()),
		nullptr,
		nullptr,
		false,
		CREATE_NO_WINDOW,
		nullptr,
		nullptr,
		&si,
		&pi
	))
	{
		log_err( "Failed to create process!" );
		return false;
	}

	CloseHandle( pi.hThread );
	CloseHandle( pi.hProcess );

	return true;
}

bool utils::other::application_focused()
{
	auto hwnd = GetForegroundWindow();
	auto dw_current_process_id = GetCurrentProcessId();
	DWORD dw_thread_process_id {};

	if (!hwnd)
		return {};

	GetWindowThreadProcessId( hwnd, &dw_thread_process_id );
	return (dw_current_process_id == dw_thread_process_id);
}

bool utils::other::is_cursor_visible()
{
	CURSORINFO ci { sizeof( CURSORINFO ) };

	if (GetCursorInfo( &ci ))
	{
		auto handle = ci.hCursor;

		if ((int( handle ) > 50000) & (int( handle ) < 100000))
			return true;
	}

	return false;
}

void utils::other::clear_strings( std::vector<std::pair<std::string, std::string>> info )
{
	for (const auto &each : info)
	{
		log_debug( "Cleaning [ %s ] on [ %s ].", each.second.c_str(), each.first.c_str() );

		auto scan = std::make_unique<scanner>( OpenProcess( PROCESS_ALL_ACCESS, FALSE, utils::other::get_process_id_by_name( each.first ) ) );
		auto u_ptrs = scan->scan_unicode( each.second );

		for (auto ptr : u_ptrs)
			scan->rewrite_unicode( ptr, " " );

		auto m_ptrs = scan->scan_multibyte( each.second );

		for (auto ptr : m_ptrs)
			scan->rewrite_multibyte( ptr, " " );
	}
}

std::string utils::other::get_disk_id()
{
	// I don't know where this code comes from, so I cannot really mention it here.

	HANDLE h_create_file = CreateFile( "\\\\.\\PhysicalDrive0", 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );

	if (h_create_file == INVALID_HANDLE_VALUE)
		return {};

	std::unique_ptr<std::remove_pointer<HANDLE>::type, void(*)(HANDLE)> h_device { h_create_file, []( HANDLE handle ) { CloseHandle( handle ); } };

	STORAGE_PROPERTY_QUERY storage_property_query {};

	storage_property_query.PropertyId = StorageDeviceProperty;
	storage_property_query.QueryType = PropertyStandardQuery;

	STORAGE_DESCRIPTOR_HEADER storage_descriptor_header {};

	DWORD dw_bytes_returned = 0;

	if (!DeviceIoControl( h_device.get(),
		IOCTL_STORAGE_QUERY_PROPERTY, &storage_property_query,
		sizeof( STORAGE_PROPERTY_QUERY ),
		&storage_descriptor_header,
		sizeof( STORAGE_DESCRIPTOR_HEADER ), &dw_bytes_returned, NULL ))
		return {};

	const DWORD dw_out_buffer_size = storage_descriptor_header.Size;

	std::unique_ptr<BYTE[]> p_out_buffer { new BYTE[dw_out_buffer_size]{} };

	if (!DeviceIoControl( h_device.get(),
		IOCTL_STORAGE_QUERY_PROPERTY, &storage_property_query,
		sizeof( STORAGE_PROPERTY_QUERY ),
		p_out_buffer.get(), dw_out_buffer_size, &dw_bytes_returned, NULL ))
		return {};

	STORAGE_DEVICE_DESCRIPTOR *p_device_descriptor =
		reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR *>(p_out_buffer.get());

	const DWORD dw_serial_number_offset = p_device_descriptor->SerialNumberOffset;

	if (dw_serial_number_offset == 0)
		return {};

	const char *serial_number =
		reinterpret_cast<const char *>(p_out_buffer.get() + dw_serial_number_offset);

	return serial_number;
}

bool utils::other::focused_situation()
{
	switch (config.clicker.version_type)
	{
		case 0: return GetForegroundWindow() == FindWindow( "LWJGL", nullptr );
		case 1: return utils::other::get_active_window_title().find( config.clicker.window_title ) != std::string::npos;
		default: return false;
	}

	// return false so we don't click where we don't want
	return false;
}

bool utils::other::get_cursor_status()
{
	/* ~~ If only in game is enabled, return if the cursor is not visible */
	if (config.clicker.only_in_game)
	{
		/* ~~ If work in inventory is enabled,
		 * ~~ return if cursor is not visible or inventory opened and cursor is visible */
		if (config.clicker.work_in_inventory)
			return !utils::other::is_cursor_visible() || (vars::key::is_inventory_opened && utils::other::is_cursor_visible());

		return !utils::other::is_cursor_visible();
	}

	/* ~~ Return true if none of those are passed so we click anywhere! */
	return true;
}