#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Core/Log.h"

#include "DebugBreak.h"

#include <filesystem>

/*
#ifdef XA_ENABLE_ASSERTS
#define XA_ASSERT(x, ...) { if(!(x)) { XA_ERROR("Assertion Failed: {0}", __VA_ARGS__); XA_DEBUGBREAK(); } }
#define XA_CORE_ASSERT(x, ...) { if(!(x)) { XA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); XA_DEBUGBREAK(); } }
#else
#define XA_ASSERT(x, ...)
#define XA_CORE_ASSERT(x, ...)
#endif
*/

#ifdef XA_ENABLE_ASSERTS

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define XA_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { XA##type##ERROR(msg, __VA_ARGS__); XA_DEBUGBREAK(); } }
	#define XA_INTERNAL_ASSERT_WITH_MSG(type, check, ...) XA_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define XA_INTERNAL_ASSERT_NO_MSG(type, check) XA_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", XA_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define XA_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define XA_INTERNAL_ASSERT_GET_MACRO(...) XA_EXPAND_MACRO( XA_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, XA_INTERNAL_ASSERT_WITH_MSG, XA_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define XA_ASSERT(...) XA_EXPAND_MACRO( XA_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define XA_CORE_ASSERT(...) XA_EXPAND_MACRO( XA_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define XA_ASSERT(...)
	#define XA_CORE_ASSERT(...)
#endif
