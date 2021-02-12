// defining stuff we need
#define JM_XORSTR_DISABLE_AVX_INTRINSICS
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

#include "def/include/imgui/imgui.h"
#include "def/include/imgui/impl/imgui_impl_dx9.h"
#include "def/include/imgui/impl/imgui_impl_win32.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "def/include/lazy.hpp"
#include "def/include/singleton.hpp"
#include "def/include/xorstr.hpp"

#include "def/utils/util.hpp"
#include "def/include/console/console.hpp"
#include "def/utils/vars.hpp"
#include "menu/ui.hpp"
#include "menu/config/config.hpp"
#include "clicker/clicker.hpp"