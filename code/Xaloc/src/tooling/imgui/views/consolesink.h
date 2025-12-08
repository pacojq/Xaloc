#include "tooling/imgui/views/console.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/details/null_mutex.h"
#include <mutex>


namespace Xaloc 
{

template<class Mutex>
class TImGuiConsoleSink : public spdlog::sinks::base_sink<std::mutex>
{
public:
	explicit TImGuiConsoleSink(bool forceFlush = false, uint8_t bufferCapacity = 10)
		: m_MessageBufferCapacity(forceFlush ? 1 : bufferCapacity), m_MessageBuffer(std::vector<ZRef<ZImGuiConsole::Message>>(forceFlush ? 1 : bufferCapacity))
	{
	}
	TImGuiConsoleSink(const TImGuiConsoleSink&) = delete;
	TImGuiConsoleSink& operator=(const TImGuiConsoleSink&) = delete;
	virtual ~TImGuiConsoleSink() = default;
protected:
	void sink_it_(const spdlog::details::log_msg& msg) override
	{
		spdlog::memory_buf_t formatted;
		base_sink<std::mutex>::formatter_->format(msg, formatted);

		*(m_MessageBuffer.begin() + m_MessagesBuffered) = std::make_shared<ZImGuiConsole::Message>(fmt::to_string(formatted), GetMessageLevel(msg.level));
		if (++m_MessagesBuffered == m_MessageBufferCapacity)
			flush_();
	}

	void flush_() override
	{
		for (ZRef<ZImGuiConsole::Message> message : m_MessageBuffer)
			ZImGuiConsole::AddMessage(message);
		m_MessagesBuffered = 0;
	}
private:
	static ZImGuiConsole::Message::Level GetMessageLevel(const spdlog::level::level_enum level)
	{
		switch (level)
		{
		case spdlog::level::level_enum::trace: return ZImGuiConsole::Message::Level::Trace;
		case spdlog::level::level_enum::debug: return ZImGuiConsole::Message::Level::Debug;
		case spdlog::level::level_enum::info: return ZImGuiConsole::Message::Level::Info;
		case spdlog::level::level_enum::warn: return ZImGuiConsole::Message::Level::Warn;
		case spdlog::level::level_enum::err: return ZImGuiConsole::Message::Level::Error;
		case spdlog::level::level_enum::critical: return ZImGuiConsole::Message::Level::Critical;
		case spdlog::level::level_enum::off: return ZImGuiConsole::Message::Level::Off;
		}
		return ZImGuiConsole::Message::Level::Invalid;
	}
private:
	uint8_t m_MessagesBuffered = 0;
	uint8_t m_MessageBufferCapacity;
	std::vector<ZRef<ZImGuiConsole::Message>> m_MessageBuffer;
};

using ZImGuiConsoleSink_mt = TImGuiConsoleSink<std::mutex>;                  // multi-threaded
using ZImGuiConsoleSink_st = TImGuiConsoleSink<spdlog::details::null_mutex>; // single threaded

}