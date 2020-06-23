#include "xapch.h"
#include "Core.h"

#include "Log.h"


#define XALOC_VERSION "v0.0.1"

namespace Xaloc {

	void InitCore()
	{
		Log::Init();

		XA_CORE_TRACE("Xaloc Engine v{0}.{1}.{2}", XALOC_VERSION_MAJOR, XALOC_VERSION_MINOR, XALOC_VERSION_PATCH);
		XA_CORE_TRACE("Initializing...");
	}

	void ShutdownCore()
	{
		XA_CORE_TRACE("Shutting down...");
	}

}