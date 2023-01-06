#include <Xaloc.h>
#include <Xaloc/EntryPoint.h>

#include "imgui/imgui.h"

#include "SandboxLayer.h"


class Sandbox : public Xaloc::Application
{
public:
	Sandbox(Xaloc::ApplicationSpec spec) : Xaloc::Application(spec)
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new SandboxLayer());

		//SetImGuiEnabled(true);
		SetImGuiEnabled(false);
	}

	~Sandbox()
	{

	}
};


Xaloc::Application* Xaloc::CreateApplication()
{
	ApplicationSpec spec;

	spec.Name = "Sandbox App";
	spec.TargetGraphics = Xaloc::RendererAPI::API::OpenGL;

	spec.UseScripting = true;
	spec.ScriptingDllPath = "assets/game/scripts/SandboxCs.dll";
	
	//spec.PauseOnFocusLost = true;

	return new Sandbox(spec);
}