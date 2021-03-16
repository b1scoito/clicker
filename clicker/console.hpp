#pragma once

enum msg_type_t : std::uint32_t
{
	LNONE = 0,
	LSUCCESS = 10,	/* green */
	LDEBUG = 11,	/* cyan */
	LWARN = 14,	/* yellow */
	LERROR = 12	/* red */
};

inline std::ostream &operator<< ( std::ostream &os, const msg_type_t type )
{
	switch ( type )
	{
		case msg_type_t::LSUCCESS:	return os << "+";
		case msg_type_t::LDEBUG:	return os << "~";
		case msg_type_t::LWARN:		return os << ">";
		case msg_type_t::LERROR:	return os << "!";
		default:					return os << "";
	};
}

class logger
{
private:
	std::shared_timed_mutex m {};

public:
	logger( )
	{
		AllocConsole( );
		AttachConsole( GetCurrentProcessId( ) );
		SetConsoleTitleA( "" );

		FILE *conin, *conout;
		freopen_s( &conin, "conin$", "r", stdin );
		freopen_s( &conout, "conout$", "w", stdout );
		freopen_s( &conout, "conout$", "w", stderr );
	}

	~logger( )
	{
		FreeConsole( );
		ShowWindow( FindWindowA( "ConsoleWindowClass", nullptr ), SW_HIDE );
	}

	template< typename ... arg >
	void print( const msg_type_t type, const std::string &format, arg ... a )
	{
		static auto *h_console = GetStdHandle( STD_OUTPUT_HANDLE );
		std::unique_lock<decltype( m )> lock( m );

		// parse and format the output
		const size_t size = static_cast< size_t >( 1 ) + std::snprintf( nullptr, 0, format.c_str( ), a ... );
		const std::unique_ptr<char[]> buf( new char[ size ] );
		std::snprintf( buf.get( ), size, format.c_str( ), a ... );
		const auto formated = std::string( buf.get( ), buf.get( ) + size - 1 );

		// print msg		
		if ( type != msg_type_t::LNONE )
		{
			SetConsoleTextAttribute( h_console, 15 /* white */ );
			std::cout << "[";

			SetConsoleTextAttribute( h_console, type );
			std::cout << type;

			SetConsoleTextAttribute( h_console, 15 /* white */ );
			std::cout << "] ";
		}

		if ( type == msg_type_t::LDEBUG )
			SetConsoleTextAttribute( h_console, 8 /* gray */ );

		std::cout << formated << std::endl;
	}
};

#ifdef _DEBUG
inline auto g_logger = std::make_unique<logger>( );
#define _log(type, ...) g_logger->print(type, __VA_ARGS__)
#else
#define _log(...)
#endif