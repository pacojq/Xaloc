#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include "Xaloc/ImGui/Modules/ImGuiConsole.h"

namespace Xaloc {


	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// CORE LOG MACROS
// if dist build, set them to do nothing
#define XA_CORE_FATAL(...)   ::Xaloc::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define XA_CORE_ERROR(...)   ::Xaloc::Log::GetCoreLogger()->error(__VA_ARGS__)
#define XA_CORE_WARN(...)    ::Xaloc::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define XA_CORE_INFO(...)    ::Xaloc::Log::GetCoreLogger()->info(__VA_ARGS__)
#define XA_CORE_TRACE(...)   ::Xaloc::Log::GetCoreLogger()->trace(__VA_ARGS__)

// CLIENT LOG MACROS
#define XA_FATAL(...)   ::Xaloc::Log::GetClientLogger()->critical(__VA_ARGS__)
#define XA_ERROR(...)   ::Xaloc::Log::GetClientLogger()->error(__VA_ARGS__)
#define XA_WARN(...)    ::Xaloc::Log::GetClientLogger()->warn(__VA_ARGS__)
#define XA_INFO(...)    ::Xaloc::Log::GetClientLogger()->info(__VA_ARGS__)
#define XA_TRACE(...)   ::Xaloc::Log::GetClientLogger()->trace(__VA_ARGS__)
