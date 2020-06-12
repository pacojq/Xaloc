#include <Xaloc.h>
#include <Xaloc/EntryPoint.h>

#include "imgui/imgui.h"

#include "EditorLayer.h"

namespace Xaloc {


	class XalocEditor : public Application
	{
	public:
		XalocEditor(ApplicationSpec spec) : Application(spec)
		{
			PushLayer(new EditorLayer());
		}

		~XalocEditor()
		{

		}
	};


	Application* CreateApplication()
	{
		ApplicationSpec spec;

		spec.Name = "Xaloc Editor";
		spec.TargetGraphics = RendererAPI::API::OpenGL;

		//spec.UseScripting = true;
		//spec.ScriptingDllPath = "assets/scripts/SandboxCs.dll";

		//spec.PauseOnFocusLost = true;

		return new XalocEditor(spec);
	}

}