#pragma once

#include "includes.hpp"

enum : int
{
	LEMPTY = 0,
	LSUCCESS,
	LDEBUG,
	LINFO,
	LWARN,
	LERROR,
	LCRITICAL,
	LMAX
};

class console : public singleton<console>
{
private:
	std::shared_timed_mutex m;

public:
	console( );
	~console( );

	void initialize( );
	void close( );

#if defined _DEBUG
	template<typename ... arg>
	void print( const int type, const std::string &format, arg ... a )
	{
		static auto h_console = LI_FN( GetStdHandle ).cached( )( STD_OUTPUT_HANDLE );
		std::unique_lock<decltype( m )> lock( m );

		const size_t size = std::snprintf( nullptr, 0, format.c_str( ), a ... ) + 1;
		const std::unique_ptr<char[]> buf( new char[ size ] );
		std::snprintf( buf.get( ), size, format.c_str( ), a ... );
		const auto str_formated = std::string( buf.get( ), buf.get( ) + size - 1 );

		static std::vector<std::pair<WORD, std::string>> tag =
		{
			{
				0,
				""
			},
			{
				FOREGROUND_GREEN | FOREGROUND_INTENSITY,
				xorstr( "SUCCESS" )
			},
			{
				0 | FOREGROUND_INTENSITY,
				xorstr( "DEBUG" )
			},
			{
				FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
				xorstr( "INFO" )
			},
			{
				FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,
				xorstr( "WARN" )
			},
			{
				FOREGROUND_RED | FOREGROUND_INTENSITY,
				xorstr( "ERROR" )
			},
			{
				BACKGROUND_RED | BACKGROUND_INTENSITY,
				xorstr( "CRITICAL" )
			},
		};

		if ( type > LEMPTY )
		{
			LI_FN( SetConsoleTextAttribute ).cached( )( h_console, 15 );
			std::cout << xorstr( "[ " );
			LI_FN( SetConsoleTextAttribute ).cached( )( h_console, tag[ type ].first );
			std::cout << tag[ type ].second.c_str( );
			LI_FN( SetConsoleTextAttribute ).cached( )( h_console, 15 );
			std::cout << xorstr( " ] - " );
		}

		if ( type == LDEBUG )
		{
			LI_FN( SetConsoleTextAttribute ).cached( )( h_console, tag[ type ].first );
			std::cout << str_formated << std::endl;
			LI_FN( SetConsoleTextAttribute ).cached( )( h_console, 15 );
		}
		else
			std::cout << str_formated << std::endl;
	}
#else
	template<typename ... arg>
	void print( const int type, const std::string &format, arg ... a )
	{
	}
#endif
};

#if defined _DEBUG
#define _log(type, ...) console::get().print(type, __VA_ARGS__)
#else
#define _log(...)
#endif