#pragma once
#include "xaloc/core/application.h"
#include "xaloc/debugging/assert.h"

/*
	Main function.
	Entry point of the application.
*/

#ifdef XA_PLATFORM_WINDOWS

extern Xaloc::ZApplication* Xaloc::CreateApplication();

int main(int argc, char** argv)
{
	Xaloc::InitCore();

	XA_PROFILE_BEGIN_SESSION("Startup", "XalocProfile-Startup.json");
	Xaloc::ZApplication* pApp = Xaloc::CreateApplication();
	XA_CORE_ASSERT(pApp, "Client Application is null!");
	XA_PROFILE_END_SESSION();

	XA_PROFILE_BEGIN_SESSION("Runtime", "XalocProfile-Runtime.json");
	pApp->Run();
	XA_PROFILE_END_SESSION();

	XA_PROFILE_BEGIN_SESSION("Shutdown", "XalocProfile-Shutdown.json");
	delete pApp;
	XA_PROFILE_END_SESSION();

	Xaloc::ShutdownCore();
}

#endif // XA_PLATFORM_WINDOWS