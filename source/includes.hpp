#include <windows.h>
#include <iostream>
#include <shared_mutex>
#include <vector>
#include <random>

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "util.hpp"
#include "lazy_importer.hpp"
#include "singleton.hpp"
#include "xorstr.hpp"
#include "console.hpp"
#include "vars.hpp"
#include "ui.hpp"
#include "config.hpp"
#include "clicker.hpp"