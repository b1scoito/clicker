#include "pch.hpp"
#include "config.hpp"

void c_config::run( std::string name )
{
	if ( PWSTR appdata_path; SUCCEEDED( SHGetKnownFolderPath( FOLDERID_RoamingAppData, 0, nullptr, &appdata_path ) ) )
	{
		path = appdata_path;
		path /= name;

		CoTaskMemFree( appdata_path );
	}

	if ( std::filesystem::is_directory( path ) )
	{
		std::transform
		(
			std::filesystem::directory_iterator { path },
			std::filesystem::directory_iterator { },
			std::back_inserter( configs ),
			[]( const auto& entry ) { return entry.path().filename().string(); }
		);
	}
}

void c_config::load( size_t id )
{
	if ( !std::filesystem::is_directory( path ) )
	{
		std::filesystem::remove( path );
		std::filesystem::create_directory( path );
	}

	std::ifstream in { path / configs[id] };

	if ( !in.good() )
		return;

	archivex<std::ifstream>{ in } >> clicker;
	in.close();
}

void c_config::save( size_t id ) const
{
	if ( !std::filesystem::is_directory( path ) )
	{
		std::filesystem::remove( path );
		std::filesystem::create_directory( path );
	}

	std::ofstream out { path / configs[id] };

	if ( !out.good() )
		return;

	archivex<std::ofstream>{ out } << clicker;
	out.close();
}

void c_config::add( std::string name )
{
	if ( !( name.empty() ) && std::find( std::cbegin( configs ), std::cend( configs ), name ) == std::cend( configs ) )
		configs.emplace_back( name );
}

void c_config::remove( size_t id )
{
	std::filesystem::remove( path / configs[id] );
	configs.erase( configs.cbegin() + id );
}

void c_config::rename( size_t item, std::string new_name )
{
	std::filesystem::rename( path / configs[item], path / new_name );
	configs[item] = new_name;
}

void c_config::reset()
{
	clicker = { };
}