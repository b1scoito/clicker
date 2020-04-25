#pragma once

#include <filesystem>

class cconfig final
{
public:
	void run( const char * );
	void load( size_t );
	void save( size_t ) const;
	void add( const char * );
	void remove( size_t );
	void rename( size_t, const char * );
	void reset( );

	constexpr auto &get_configs( )
	{
		return configs;
	}

	struct
	{
		bool enabled { false };
		std::string window_name;
		int index_version { 0 };
		float min_cps { 0.f };
		float max_cps { 0.f };
	} clicker;

private:
	std::filesystem::path path;
	std::vector<std::string> configs;
};

extern cconfig config;