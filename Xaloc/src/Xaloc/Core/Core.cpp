#include "xapch.h"
#include "Core.h"

#include "Log.h"

#define XALOC_VERSION "v0.0.1"

namespace Xaloc {

	void InitCore()
	{
		Log::Init();

		XA_CORE_TRACE("Xaloc Engine {}", XALOC_VERSION);
		XA_CORE_TRACE("Initializing...");
	}

	void ShutdownCore()
	{
		XA_CORE_TRACE("Shutting down...");
	}

}