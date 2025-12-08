#include "xaloc/entrypoint.h"
#include "xaloc/core/application.h"
#include "editor/module.h"
#include "imgui/imgui.h"


namespace Editor
{

class ZXalocEditor : public Xaloc::ZApplication
{
public:
	ZXalocEditor(Xaloc::SApplicationSpec spec) : ZApplication(spec)
	{
		PushModule(new ZModuleEditor());
	}

	~ZXalocEditor()
	{
	}
};

}

namespace Xaloc
{

ZApplication* CreateApplication()
{
	Xaloc::SApplicationSpec spec;

	spec.sName = "Xaloc Editor";
	spec.eTargetGraphics = Xaloc::ERenderAPI::OpenGL;

	spec.bUseScripting = true;
	spec.pathScriptingDll = "assets/game/scripts/SandboxCs.dll";

	//spec.PauseOnFocusLost = true;

	return new Editor::ZXalocEditor(spec);
}

}