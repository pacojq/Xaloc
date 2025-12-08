#include "xapch.h"
#include "xaloc/core/application.h"
#include "xaloc/core/log.h"
#include "xaloc/core/assets/manager.h"
#include "xaloc/events/event.h"
#include "xaloc/events/application.h"
#include "xaloc/renderer/renderer.h"
#include "xaloc/scripting/engine.h"
#include "tooling/imgui/style.h"
#include <GLFW/glfw3.h> // TODO: We use GLFW to get the timestep. We should make it platform-specific.


namespace Xaloc
{

#define BIND_EVENT_FN(x) std::bind(&ZApplication::x, this, std::placeholders::_1)

ZApplication* ZApplication::s_pInstance = nullptr;

ZApplication::ZApplication(SApplicationSpec spec)
	: m_sName(spec.sName)
{
	XA_PROFILE_FUNCTION();

	XA_CORE_ASSERT(!s_pInstance, "Application should be null");
	s_pInstance = this;

	m_bPauseOnFocusLost = spec.bPauseOnFocusLost;

	ZAssetManager::Init(spec.pathAssetsRoot);
	ZAssetManager::LoadProjectAssets(); // TODO: revise this if we are loading packed release assets

	SWindowDefinition window;
	window.sTitle = spec.sName;
	window.nWidth = spec.nWidth;
	window.nHeight = spec.nHeight;
	window.bVSyncEnabled = spec.bEnableVSync;

	m_Window = std::unique_ptr<ZWindow>(ZWindow::Create(spec.eTargetGraphics, window));
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

	RendererInit(spec.eTargetGraphics);

	// Setup ImGui
	m_pModuleImGui = ZModuleImGui::Create();
	PushModuleOverlay(m_pModuleImGui);
	m_Modules.Flush();
	ImGuiApplyDefaultStyle();

	// Setup C# scripting modules
	if (spec.bUseScripting)
	{
		// TODO error handling with ScriptingDllPath
		Scripting::ZScriptEngine::Init(spec.pathScriptingDll);
	}
}

ZApplication::~ZApplication()
{
	XA_PROFILE_FUNCTION();
	ZAssetManager::Shutdown();
	RendererShutdown();
}

void ZApplication::OnEvent(ZEvent& rEvent)
{
	XA_PROFILE_FUNCTION();

	ZEventDispatcher dispatcher(rEvent);
	dispatcher.Dispatch<EvtWindowClose>(BIND_EVENT_FN(OnWindowClose));
	dispatcher.Dispatch<EvtWindowResize>(BIND_EVENT_FN(OnWindowResize));
	dispatcher.Dispatch<EvtWindowFocus>(BIND_EVENT_FN(OnWindowFocus));

	// Dispatch events down to our layer stack
	for (auto it = m_Modules.end(); it != m_Modules.begin();)
	{
		if (rEvent.bHandled)
		{
			break;
		}
		(*--it)->OnEvent(rEvent);
	}
}

void ZApplication::PushModule(ZModule* pModule)
{
	XA_PROFILE_FUNCTION();
	m_Modules.Push(pModule);
}

void ZApplication::PopModule(ZModule* pModule)
{
	XA_PROFILE_FUNCTION();
	m_Modules.Pop(pModule);
}

void ZApplication::PushModuleOverlay(ZModule* pModule)
{
	XA_PROFILE_FUNCTION();
	m_Modules.PushOverlay(pModule);
}

void ZApplication::Run()
{
	XA_PROFILE_FUNCTION();

	m_Modules.Flush();

	while (m_bRunning)
	{
		XA_PROFILE_SCOPE("RunLoop");

		float time = (float)glfwGetTime(); // TODO Platform::GetTime()
		ZTimestep timestep = time - m_fLastFrameTime;
		m_fLastFrameTime = time;

		m_fFPS = 1.0f / timestep;
		m_fTimestep = timestep * 1000.0f; // seconds to milliseconds

		if (!m_bMinimized)
		{
			if (!m_bPaused)
			{
				m_pModuleImGui->GetProfiler()->BeginFrame();
				XA_PROFILE_SCOPE("Modules OnUpdate");
				for (ZModule* pModule : m_Modules)
				{
					pModule->OnUpdate(timestep);
				}
				m_pModuleImGui->GetProfiler()->EndFrame();
			}

			if (m_bImGuiEnabled)
			{
				m_pModuleImGui->Begin();
				XA_PROFILE_SCOPE("Modules OnImGuiRender");
				for (ZModule* pModule : m_Modules)
				{
					pModule->OnImGuiRender();
				}
				m_pModuleImGui->End();
			}

			m_Modules.Flush();
		}

		m_Window->OnUpdate();
	}

	// TODO Shutdown
	Scripting::ZScriptEngine::Shutdown();
}

void ZApplication::Close()
{
	m_bRunning = false;
}

bool ZApplication::OnWindowClose(EvtWindowClose& rEvent)
{
	Close();
	return true;
}

// TODO: don't use window resize callback - use viewport callback!
bool ZApplication::OnWindowResize(EvtWindowResize& rEvent)
{
	XA_PROFILE_FUNCTION();

	if (rEvent.GetWidth() == 0 || rEvent.GetHeight() == 0)
	{
		m_bMinimized = true;
		return false;
	}
	m_bMinimized = false;
	RendererSetViewport(rEvent.GetWidth(), rEvent.GetHeight());
	return true;
}

bool ZApplication::OnWindowFocus(EvtWindowFocus& rEvent)
{
	XA_CORE_INFO(rEvent.IsFocused()
		? "Window focus gain registered."
		: "Window focus loss registered.");

	if (m_bPauseOnFocusLost)
	{
		if (rEvent.IsFocused() && !m_bFocused)
		{
			m_bPaused = false;
		}
		else if (!rEvent.IsFocused() && m_bFocused)
		{
			m_bPaused = true;
		}
	}
	m_bFocused = rEvent.IsFocused();
	return false;
}

}
