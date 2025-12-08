#include "xapch.h"
#include "xaloc/debugging/instrumentor.h"
#include <algorithm>


namespace Xaloc
{

static ZInstrumentor s_instance;

static void WriteHeader(std::ofstream& rStream)
{
	rStream << "{\"otherData\": {},\"traceEvents\":[{}";
	rStream.flush();
}

static void WriteFooter(std::ofstream& rStream)
{
	rStream << "]}";
	rStream.flush();
}

ZInstrumentor::ZInstrumentor()
	: m_pCurrentSession(nullptr)
{
}

void ZInstrumentor::BeginSession(
	const std::string& sName,
	const std::string& pathOutput)
{
	std::lock_guard lock(m_mutex);
	if (m_pCurrentSession)
	{
		// If there is already a current session, then close it before beginning new one.
		// Subsequent profiling output meant for the original session will end up in the
		// newly opened session instead.  That's better than having badly formatted
		// profiling output.
		if (ZLog::GetCore()) // Edge case: BeginSession() might be called before ZLog::Init() - TODO: fix
		{
			XA_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.",
				sName,
				m_pCurrentSession->sName);
		}
		InternalEndSession();
	}
	m_OutputStream.open(pathOutput);

	if (m_OutputStream.is_open())
	{
		m_pCurrentSession = new SInstrumentationSession({ sName });
		WriteHeader(m_OutputStream);
	}
	else
	{
		if (ZLog::GetCore()) // Edge case: BeginSession() might be called before ZLog::Init() - TODO: fix
		{
			XA_CORE_ERROR("Instrumentor could not open results file '{0}'.", pathOutput);
		}
	}
}

void ZInstrumentor::EndSession()
{
	std::lock_guard lock(m_mutex);
	InternalEndSession();
}

void ZInstrumentor::WriteProfile(
	const SProfileResult& result)
{
	std::stringstream json;

	json << std::setprecision(3) << std::fixed;
	json << ",{";
	json << "\"cat\":\"function\",";
	json << "\"dur\":" << (result.timeElapsed.count()) << ',';
	json << "\"name\":\"" << result.sName << "\",";
	json << "\"ph\":\"X\",";
	json << "\"pid\":0,";
	json << "\"tid\":" << result.nThreadID << ",";
	json << "\"ts\":" << result.start.count();
	json << "}";

	std::lock_guard lock(m_mutex);
	if (m_pCurrentSession)
	{
		m_OutputStream << json.str();
		m_OutputStream.flush();
	}
}

ZInstrumentor& ZInstrumentor::Get()
{
	return s_instance;
}

void ZInstrumentor::InternalEndSession()
{
	if (m_pCurrentSession)
	{
		WriteFooter(m_OutputStream);
		m_OutputStream.close();
		delete m_pCurrentSession;
		m_pCurrentSession = nullptr;
	}
}

ZInstrumentationTimer::ZInstrumentationTimer(
	const char* sName)
	: m_sName(sName)
	, m_bStopped(false)
{
}

ZInstrumentationTimer::~ZInstrumentationTimer()
{
	if (!m_bStopped)
	{
		Stop();
	}
}

void ZInstrumentationTimer::Stop()
{
	auto endTimepoint = std::chrono::steady_clock::now();
	auto highResStart = FloatingPointMicroseconds{ m_timeStart.time_since_epoch() };
	auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_timeStart).time_since_epoch();

	ZInstrumentor::Get().WriteProfile({ m_sName, highResStart, elapsedTime, std::this_thread::get_id() });

	m_bStopped = true;
}

}