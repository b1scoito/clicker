#pragma once

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN 

#include <iostream>
#include <thread>
#include <random>

#include <Windows.h>

#include "singleton.hpp"
#include "console.hpp"
#include "config.hpp"
#include "utils.hpp"
#include "vars.hpp"
#include "hooks.hpp"