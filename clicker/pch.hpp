#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers 
#define NOMINMAX // std::min() and std::max() instead of min() and max()

#include <iostream>
#include <thread>
#include <random>
#include <windows.h>

using namespace std::chrono_literals;

#include "console.hpp"
#include "keybind.hpp"
#include "config.hpp"
#include "vars.hpp"
#include "utils.hpp"
#include "clicker.hpp"
#include "threads.hpp"
#include "menu.hpp"