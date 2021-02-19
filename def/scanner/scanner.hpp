#pragma once

// https://github.com/diegcrane/external-memory-scanner

#include <windows.h>
#include <vector>
#include <iostream>
#pragma comment(lib, "ntdll.lib")

#include <winternl.h> 
#include "../console.hpp"

#define NT_SUCCESS(x) ((NTSTATUS)(x) >= NULL)

struct scanner
{
private:
	HANDLE p_handle = NULL;
	size_t _address = NULL;

	MEMORY_BASIC_INFORMATION _mbi = {
		nullptr,
		nullptr,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL

	};

public:
	scanner( HANDLE p_handle );
	~scanner( );

	bool query_memory( );
	bool is_valid_page( );
	bool read_virtual_mem( PVOID x );

	std::vector<size_t> scan_unicode( std::string string );
	void rewrite_unicode( size_t addrss, std::string str );

	std::vector<size_t> scan_multibyte( std::string string );
	void rewrite_multibyte( size_t addrss, std::string str );

};

extern "C" NTSTATUS ZwReadVirtualMemory( HANDLE, PVOID, PVOID, SIZE_T, SIZE_T * );
extern "C" NTSTATUS ZwWriteVirtualMemory( HANDLE, PVOID, PVOID, SIZE_T, SIZE_T * );