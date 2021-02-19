#include "scanner.hpp"

scanner::scanner( HANDLE p_handle )
{
	this->p_handle = p_handle;
}

scanner::~scanner( )
{
	CloseHandle( this->p_handle );
}

bool scanner::query_memory( )
{
	return VirtualQueryEx(
		this->p_handle, ( LPVOID ) this->_address, &this->_mbi, sizeof( this->_mbi ) );
}

bool scanner::is_valid_page( )
{
	return this->_mbi.State == MEM_COMMIT && this->_mbi.Protect == PAGE_READWRITE;
}

bool scanner::read_virtual_mem( PVOID x )
{
	return NT_SUCCESS( NtReadVirtualMemory(
		this->p_handle, ( LPVOID ) this->_address, x, this->_mbi.RegionSize, nullptr ) );
}

std::vector<size_t> scanner::scan_unicode( std::string string )
{
	size_t ___length = string.length( ); std::vector<size_t> _mem_locations;

	for ( this->_address = 0; this->query_memory( ); this->_address += this->_mbi.RegionSize )
	{
		if ( !this->is_valid_page( ) )
			continue;

		std::vector<WCHAR> buffer( this->_mbi.RegionSize );

		if ( !this->read_virtual_mem( &buffer[ 0 ] ) )
			continue;

		for ( size_t x = 0; x < this->_mbi.RegionSize / 2; ++x )
		{
			for ( size_t y = 0; y < ___length; ++y )
			{
				if ( buffer[ x + y ] != string[ y ] )
					break;

				if ( y == ___length - 1 )
				{
					_mem_locations.push_back( this->_address + x * 2 );
					_log( LDEBUG, "[ string_scan ] Read unicode: 0x%x", _address + x * 2 );
				}
			}
		}
	}

	return _mem_locations;
}

std::vector<size_t> scanner::scan_multibyte( std::string string )
{
	size_t ___length = string.length( ); std::vector<size_t> _mem_locations;

	for ( this->_address = 0; this->query_memory( ); this->_address += this->_mbi.RegionSize )
	{
		if ( !this->is_valid_page( ) )
			continue;

		std::vector<char> buffer( this->_mbi.RegionSize );

		if ( !this->read_virtual_mem( &buffer[ 0 ] ) )
			continue;

		for ( size_t x = 0; x < this->_mbi.RegionSize; ++x )
		{
			for ( size_t y = 0; y < ___length; ++y )
			{
				if ( buffer[ x + y ] != string[ y ] )
					break;

				if ( y == ___length - 1 )
				{
					_mem_locations.push_back( this->_address + x );
					_log( LDEBUG, "[ string_scan ] Read multibyte: 0x%x", _address + x );
				}
			}
		}
	}

	return _mem_locations;
}

void scanner::rewrite_unicode( size_t addr, std::string str )
{
	for ( size_t x = 0; x < str.length( ); ++x )
	{
		NtWriteVirtualMemory( this->p_handle, ( LPVOID ) ( addr + x * 2 ), &str[ x ], 1, nullptr );
	}

	_log( LDEBUG, "[ string_scan ] Write unicode: 0x%x", addr );
}


void scanner::rewrite_multibyte( size_t addr, std::string str )
{
	for ( size_t x = 0; x < str.length( ); ++x )
	{
		NtWriteVirtualMemory( this->p_handle, ( LPVOID ) ( addr + x ), &str[ x ], 1, nullptr );
	}

	_log( LDEBUG, "[ string_scan ] Write multibyte: 0x%x", addr );
}