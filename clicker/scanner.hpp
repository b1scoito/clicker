#pragma once

// https://github.com/diegcrane/external-memory-scanner

struct scanner
{
private:
	HANDLE h_process {};
	size_t address {};

	MEMORY_BASIC_INFORMATION mem_basic_info {};

public:
	scanner( HANDLE h_process );
	~scanner();

	bool query_memory();
	bool is_valid_page();
	bool read_virtual_mem( PVOID buffer );

	std::vector<size_t> scan_unicode( std::string string );
	void rewrite_unicode( size_t address, std::string string );

	std::vector<size_t> scan_multibyte( std::string string );
	void rewrite_multibyte( size_t address, std::string string );
};