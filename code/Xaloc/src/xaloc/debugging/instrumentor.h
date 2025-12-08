#pragma once
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>


namespace Xaloc
{

// TODO: migrate to Tracy Profiler

using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct SProfileResult
{
	std::string sName;

	FloatingPointMicroseconds start;
	std::chrono::microseconds timeElapsed;
	std::thread::id nThreadID;
};

struct SInstrumentationSession
{
	std::string sName;
};

/// <summary>
/// Instrumentor class that outputs JSON files to be read by the chrome://tracing application
/// </summary>
class ZInstrumentor
{
public:
	ZInstrumentor();

	void BeginSession(
		const std::string& sName,
		const std::string& pathOutput = "results.json");

	void EndSession();

	void WriteProfile(
		const SProfileResult& result);

	static ZInstrumentor& Get();

private:

	// Note: you must already own lock on m_Mutex before
	// calling InternalEndSession()
	void InternalEndSession();

private:
	std::mutex m_mutex;
	SInstrumentationSession* m_pCurrentSession;
	std::ofstream m_OutputStream;
};

class ZInstrumentationTimer
{
public:
	ZInstrumentationTimer(const char* sName);

	~ZInstrumentationTimer();

	void Stop();

private:
	const char* m_sName;
	std::chrono::time_point<std::chrono::steady_clock> m_timeStart;
	bool m_bStopped;
};

namespace InstrumentorUtils
{
	template <size_t N>
	struct ChangeResult
	{
		char Data[N];
	};

	template <size_t N, size_t K>
	constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
	{
		ChangeResult<N> result = {};

		size_t srcIndex = 0;
		size_t dstIndex = 0;
		while (srcIndex < N)
		{
			size_t matchIndex = 0;
			while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
				matchIndex++;
			if (matchIndex == K - 1)
				srcIndex += matchIndex;
			result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
			srcIndex++;
		}
		return result;
	}
}

}

#define XA_PROFILE 0
#if XA_PROFILE

	// Resolve which function signature macro will be used. Note that this only
	// is resolved when the (pre)compiler starts, so the syntax highlighting
	// could mark the wrong one in your editor!
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define XA_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define XA_FUNC_SIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define XA_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define XA_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define XA_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define XA_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define XA_FUNC_SIG __func__
	#else
		#define XA_FUNC_SIG "XA_FUNC_SIG unknown!"
	#endif

	#define XA_PROFILE_BEGIN_SESSION(name, filepath) ::Xaloc::ZInstrumentor::Get().BeginSession(name, filepath)
	#define XA_PROFILE_END_SESSION() ::Xaloc::ZInstrumentor::Get().EndSession()
	#define XA_PROFILE_SCOPE(name) constexpr auto fixedName = ::Xaloc::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
										::Xaloc::ZInstrumentationTimer timer##__LINE__(fixedName.Data)
	#define XA_PROFILE_FUNCTION() XA_PROFILE_SCOPE(XA_FUNC_SIG)

#else

	#define XA_PROFILE_BEGIN_SESSION(name, filepath)
	#define XA_PROFILE_END_SESSION()
	#define XA_PROFILE_SCOPE(name)
	#define XA_PROFILE_FUNCTION()

#endif