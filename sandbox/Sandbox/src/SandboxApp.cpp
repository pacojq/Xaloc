#include "xaloc/entrypoint.h"
#include "xaloc/core/application.h"
#include "xaloc/core/module.h"
#include "module.h"

using namespace Xaloc;


class Sandbox : public ZApplication
{
public:
	Sandbox(Xaloc::SApplicationSpec spec) : ZApplication(spec)
	{
		//PushLayer(new ExampleLayer());
		PushModule(new ZModuleSandbox());

		//SetImGuiEnabled(true);
		SetImGuiEnabled(false);
	}

	~Sandbox()
	{
	}
};

Xaloc::ZApplication* Xaloc::CreateApplication()
{
	SApplicationSpec spec;

	spec.sName = "Sandbox App";
	spec.eTargetGraphics = ERenderAPI::OpenGL;

	spec.bUseScripting = true;
	spec.pathScriptingDll = "assets/game/scripts/SandboxCs.dll";
	
	//spec.PauseOnFocusLost = true;

	return new Sandbox(spec);
}