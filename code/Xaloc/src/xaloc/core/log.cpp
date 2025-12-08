#include "xapch.h"
#include "xaloc/core/log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "tooling/imgui/views/console.h"
#include "tooling/imgui/views/consolesink.h"


namespace Xaloc
{

std::shared_ptr<spdlog::logger> ZLog::s_pLoggerCore;
std::shared_ptr<spdlog::logger> ZLog::s_pLoggerClient;

void ZLog::Init()
{
	// create the sinks
	std::vector<spdlog::sink_ptr> sinks;
	sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>()); // VS debug console
	sinks.emplace_back(std::make_shared<ZImGuiConsoleSink_mt>(true)); // ImGuiConsole

	// create the loggers
	s_pLoggerCore = std::make_shared<spdlog::logger>("XALOC", begin(sinks), end(sinks));
	s_pLoggerClient = std::make_shared<spdlog::logger>("APP  ", begin(sinks), end(sinks));
	
	spdlog::register_logger(s_pLoggerCore);
	spdlog::register_logger(s_pLoggerClient);

	spdlog::set_pattern("%^[%T] %n: %v%$");
	s_pLoggerCore->set_level(spdlog::level::trace);
	s_pLoggerClient->set_level(spdlog::level::trace);
}

spdlog::logger* ZLog::GetCore()
{
	return s_pLoggerCore.get();
}

spdlog::logger* ZLog::GetClient()
{
	return s_pLoggerClient.get();
}

}