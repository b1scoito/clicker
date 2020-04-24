#pragma once

#include "math.hpp"

int math::random_int( int i_start, int i_end )
{
	std::random_device rd;
	std::mt19937 rng( rd( ) );
	const std::uniform_int_distribution<int> uni( i_start, i_end );

	return static_cast< int >( uni( rng ) );
}