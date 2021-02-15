#pragma once

#include "../def/includes.hpp"
#include "../mouse/mouse.hpp"

class clicker
{
public:
	void work( );

	~clicker( ) = default;
	clicker( ) = default;
};

inline auto g_clicker = std::make_unique<clicker>( );