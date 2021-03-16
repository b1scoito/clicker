#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define NOMINMAX // Use std::min and std::max instead of min() and max()

// Windows includes
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <shared_mutex>
#include <vector>
#include <random>
#include <unordered_map>
#include <string>
#include <thread>
#include <shellapi.h>

// config
#include "config.hpp"
// imgui
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
// directx9 library
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
// imgui fonts
#include "font_definitions.hpp"
#include "font_awesome.hpp"
// memory scanner
#include "scanner.hpp"
// console logger
#include "console.hpp"
// global vars
#include "vars.hpp"
// utility functions
#include "util.hpp"
// clicker thread
#include "clicker.hpp"
// hooks thread
#include "hooks.hpp"
// menu thread
#include "menu.hpp"
