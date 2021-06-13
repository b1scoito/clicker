#pragma once

#include <shared_mutex>

enum class msg_type_t: std::uint32_t
{
	LNONE = 0,
	LSUCCESS = 10,	/* green */
	LDEBUG = 9,		/* blue */
	LWARN = 14,		/* yellow */
	LERROR = 12		/* red */
};

inline std::ostream& operator<< ( std::ostream& os, const msg_type_t type )
{
	switch ( type )
	{
		case msg_type_t::LSUCCESS:	return os << ">>";
		case msg_type_t::LDEBUG:	return os << "..";
		case msg_type_t::LWARN:		return os << "**";
		case msg_type_t::LERROR:	return os << "!!";
		default: return os << "";
	}
}

class logger
{
private:
	std::shared_timed_mutex mutex {};

public:
	logger( const std::wstring title_name = {} )
	{
		AllocConsole();
		AttachConsole( GetCurrentProcessId() );

		if ( !title_name.empty() )
		{
			SetConsoleTitle( title_name.data() );
		}

		FILE* conin, * conout;

		freopen_s( &conin, "conin$", "r", stdin );
		freopen_s( &conout, "conout$", "w", stdout );
		freopen_s( &conout, "conout$", "w", stderr );
	}

	~logger()
	{
		const auto handle = FindWindow( L"ConsoleWindowClass", nullptr );
		ShowWindow( handle, SW_HIDE );
		FreeConsole();
	}

	template< typename ... arg >
	void print( const msg_type_t type, const std::string& func, const std::string& format, arg ... a )
	{
		static auto* h_console = GetStdHandle( STD_OUTPUT_HANDLE );
		std::unique_lock<decltype( mutex )> lock( mutex );

		const size_t size = static_cast<size_t>( 1 ) + std::snprintf( nullptr, 0, format.c_str(), a ... );
		const std::unique_ptr<char[]> buf( new char[size] );
		std::snprintf( buf.get(), size, format.c_str(), a ... );
		const auto formated = std::string( buf.get(), buf.get() + size - 1 );

		if ( type != msg_type_t::LNONE )
		{
			SetConsoleTextAttribute( h_console, static_cast<WORD>( type ) );
			std::cout << "[";
			std::cout << type;
			std::cout << "] ";

			SetConsoleTextAttribute( h_console, 15 /* white */ );
			std::cout << "[ ";

			SetConsoleTextAttribute( h_console, static_cast<WORD>( type ) );
			std::cout << func;

			SetConsoleTextAttribute( h_console, 15 /* white */ );
			std::cout << " ] ";
		}

		if ( type == msg_type_t::LDEBUG )
			SetConsoleTextAttribute( h_console, 8 /* gray */ );
		else
			SetConsoleTextAttribute( h_console, 15 /* white */ );

		std::cout << formated << "\n";
	}
};

#ifdef _DEBUG
inline auto g_logger = logger( L"-> clicker debug" );
#define log_raw(...) g_logger.print( msg_type_t::LNONE, __FUNCTION__, __VA_ARGS__ )
#define log_ok(...) g_logger.print( msg_type_t::LSUCCESS, __FUNCTION__, __VA_ARGS__ )
#define log_debug(...) g_logger.print( msg_type_t::LDEBUG, __FUNCTION__, __VA_ARGS__ )
#define log_warn(...) g_logger.print( msg_type_t::LWARN, __FUNCTION__, __VA_ARGS__ )
#define log_err(...) g_logger.print( msg_type_t::LERROR, __FUNCTION__, __VA_ARGS__ )
#else
#define log_raw(...)
#define log_ok(...)
#define log_debug(...)
#define log_warn(...)
#define log_err(...)
#endif