#pragma once

#include "includes.hpp"
#include "archivex.hpp"

#include <ShlObj.h>
#include <fstream>
#include <filesystem>

class c_config final
{
public:
	void run( const char * );
	void load( size_t );
	void save( size_t ) const;
	void add( const char * );
	void remove( size_t );
	void rename( size_t, const char * );
	void reset( );
	constexpr auto &get_configs( ) { return configs; }

	struct
	{
		bool enabled { false };
		int enabled_key { false };
		int activation_type { 0 };
		int version_type { 0 };
		int min_cps { 0 };
		int max_cps { 0 };

		std::string window_title;
	}
	clicker;

private:
	std::filesystem::path path;
	std::vector<std::string> configs;
};

extern c_config config;