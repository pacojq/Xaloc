#pragma once

#ifdef XA_PLATFORM_WINDOWS

	#ifndef NOMINMAX
	#define NOMINMAX 1
	#endif
	#include <Windows.h>

#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Xaloc/Core/Log.h"

#include "Xaloc/Debug/Instrumentor.h"