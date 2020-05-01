// defining stuff we need
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// includes
#include <windows.h>
#include <iostream>
#include <shared_mutex>
#include <vector>
#include <random>
#include <unordered_map>
#include <string>

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "lazy_importer.hpp"
#include "singleton.hpp"
#include "xorstr.hpp"

#include "util.hpp"
#include "console.hpp"
#include "vars.hpp"
#include "ui.hpp"
#include "config.hpp"
#include "clicker.hpp"