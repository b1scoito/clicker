#include "pch.hpp"
#include "config.hpp"

auto c_config::run( std::string name ) -> void
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

auto c_config::load( size_t id ) -> void
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

auto c_config::save( size_t id ) const -> void
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

auto c_config::add( std::string name ) -> void
{
	if ( !( name.empty() ) && std::find( std::cbegin( configs ), std::cend( configs ), name ) == std::cend( configs ) )
		configs.emplace_back( name );
}

auto c_config::remove( size_t id ) -> void
{
	std::filesystem::remove( path / configs[id] );
	configs.erase( configs.cbegin() + id );
}

auto c_config::rename( size_t item, std::string new_name ) -> void
{
	std::filesystem::rename( path / configs[item], path / new_name );
	configs[item] = new_name;
}

auto c_config::reset() -> void
{
	clicker = { };
}