#pragma once
#include "xaloc/types.h"
#include "xaloc/core/core.h"

#define FMT_UNICODE 0
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Xaloc
{

class ZLog
{
public:
    static void Init();

    static spdlog::logger* GetCore();
    static spdlog::logger* GetClient();

private:
    static std::shared_ptr<spdlog::logger> s_pLoggerCore;
    static std::shared_ptr<spdlog::logger> s_pLoggerClient;
};

}

// CORE LOG MACROS
// TODO: if dist build, set them to do nothing
#define XA_CORE_FATAL(...)   ::Xaloc::ZLog::GetCore()->critical(__VA_ARGS__)
#define XA_CORE_ERROR(...)   ::Xaloc::ZLog::GetCore()->error(__VA_ARGS__)
#define XA_CORE_WARN(...)    ::Xaloc::ZLog::GetCore()->warn(__VA_ARGS__)
#define XA_CORE_INFO(...)    ::Xaloc::ZLog::GetCore()->info(__VA_ARGS__)
#define XA_CORE_TRACE(...)   ::Xaloc::ZLog::GetCore()->trace(__VA_ARGS__)

// CLIENT LOG MACROS
// TODO: if dist build, set them to do nothing
#define XA_FATAL(...)   ::Xaloc::ZLog::GetClient()->critical(__VA_ARGS__)
#define XA_ERROR(...)   ::Xaloc::ZLog::GetClient()->error(__VA_ARGS__)
#define XA_WARN(...)    ::Xaloc::ZLog::GetClient()->warn(__VA_ARGS__)
#define XA_INFO(...)    ::Xaloc::ZLog::GetClient()->info(__VA_ARGS__)
#define XA_TRACE(...)   ::Xaloc::ZLog::GetClient()->trace(__VA_ARGS__)
