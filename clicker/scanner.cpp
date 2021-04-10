#include "pch.hpp"
#include "scanner.hpp"

scanner::scanner( HANDLE h_process )
{
	this->h_process = h_process;
}

scanner::~scanner()
{
	CloseHandle( this->h_process );
}

bool scanner::query_memory()
{
	return VirtualQueryEx(
		this->h_process,
		(LPVOID) this->address,
		&this->mem_basic_info,
		sizeof( this->mem_basic_info )
	);
}

bool scanner::is_valid_page()
{
	return this->mem_basic_info.State == MEM_COMMIT
		&& this->mem_basic_info.Protect == PAGE_READWRITE;
}

bool scanner::read_virtual_mem( PVOID buffer )
{
	return ReadProcessMemory(
		this->h_process,
		(LPVOID) this->address,
		buffer,
		static_cast<ULONG>(this->mem_basic_info.RegionSize),
		nullptr
	);
}

std::vector<size_t> scanner::scan_unicode( std::string string )
{
	size_t len = string.length();
	std::vector<size_t> memory_locations;

	for (this->address = 0; this->query_memory(); this->address += this->mem_basic_info.RegionSize)
	{
		if (!this->is_valid_page())
			continue;

		std::vector<WCHAR> buffer( this->mem_basic_info.RegionSize );

		if (!this->read_virtual_mem( &buffer[0] ))
			continue;

		for (size_t x = 0; x < this->mem_basic_info.RegionSize / 2; ++x)
		{
			for (size_t y = 0; y < len; ++y)
			{
				if (buffer[x + y] != string[y])
					break;

				if (y == len - 1)
				{
					memory_locations.push_back( this->address + x * 2 );
					log_debug( "Reading unicode address [ 0x%x ]", this->address + x * 2 );
				}
			}
		}
	}

	return memory_locations;
}

std::vector<size_t> scanner::scan_multibyte( std::string string )
{
	size_t len = string.length();
	std::vector<size_t> memory_locations;

	for (this->address = 0; this->query_memory(); this->address += this->mem_basic_info.RegionSize)
	{
		if (!this->is_valid_page())
			continue;

		std::vector<char> buffer( this->mem_basic_info.RegionSize );

		if (!this->read_virtual_mem( &buffer[0] ))
			continue;

		for (size_t x = 0; x < this->mem_basic_info.RegionSize; ++x)
		{
			for (size_t y = 0; y < len; ++y)
			{
				if (buffer[x + y] != string[y])
					break;

				if (y == len - 1)
				{
					memory_locations.push_back( this->address + x );
					log_debug( "Reading multibyte address [ 0x%x ]", this->address + x );
				}
			}
		}
	}

	return memory_locations;
}

void scanner::rewrite_unicode( size_t address, std::string string )
{
	for (size_t x = 0; x < string.length(); ++x)
		WriteProcessMemory(
			this->h_process,
			(LPVOID) (address + x * 2),
			&string[x],
			1,
			nullptr
		);

	log_debug( "Writing unicode memory at address [ 0x%x ]", address );
}


void scanner::rewrite_multibyte( size_t address, std::string string )
{
	for (size_t x = 0; x < string.length(); ++x)
		WriteProcessMemory(
			this->h_process,
			(LPVOID) (address + x),
			&string[x],
			1,
			nullptr
		);

	log_debug( "Writing multibyte memory at address [ 0x%x ]", address );
}