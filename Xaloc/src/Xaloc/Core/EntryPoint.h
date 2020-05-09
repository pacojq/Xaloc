#pragma once

/*
	Main function.
	Entry point of the application.
*/


#ifdef XA_PLATFORM_WINDOWS

extern Xaloc::Application* Xaloc::CreateApplication();

int main(int argc, char** argv)
{
	Xaloc::Log::Init();
	XA_CORE_WARN("Initialized Log!");
	XA_INFO("Initialized Log!");

	auto app = Xaloc::CreateApplication();
	app->Run();
	delete app;
}

#endif // XA_PLATFORM_WINDOWS