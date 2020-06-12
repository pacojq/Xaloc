#pragma once
#include "Core/Application.h"

/*
	Main function.
	Entry point of the application.
*/


#ifdef XA_PLATFORM_WINDOWS

extern Xaloc::Application* Xaloc::CreateApplication();

int main(int argc, char** argv)
{
	Xaloc::InitCore();

	XA_PROFILE_BEGIN_SESSION("Startup", "XalocProfile-Startup.json");
	auto app = Xaloc::CreateApplication();
	XA_CORE_ASSERT(app, "Client Application is null!");
	XA_PROFILE_END_SESSION();

	XA_PROFILE_BEGIN_SESSION("Runtime", "XalocProfile-Runtime.json");
	app->Run();
	XA_PROFILE_END_SESSION();

	XA_PROFILE_BEGIN_SESSION("Shutdown", "XalocProfile-Shutdown.json");
	delete app;
	XA_PROFILE_END_SESSION();

	Xaloc::ShutdownCore();
}

#endif // XA_PLATFORM_WINDOWS