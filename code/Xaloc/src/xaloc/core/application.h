#pragma once
#include "xaloc/types.h"
#include "xaloc/core/core.h"
#include "xaloc/core/modulestack.h"
#include "xaloc/core/time.h"
#include "xaloc/core/window.h"
#include "xaloc/renderer/renderer.h"
#include "tooling/imgui/module.h"

namespace Xaloc {
	class ZEvent;
	class EvtWindowClose;
	class EvtWindowResize;
	class EvtWindowFocus;
}


namespace Xaloc
{

// TODO: use ini config files
struct SApplicationSpec
{
	std::string sName = "Xaloc App";

	ERenderAPI eTargetGraphics = ERenderAPI::OpenGL;

	/// <summary>
	/// Default width of the application display.
	/// </summary>
	uint32 nWidth = 1280;

	/// <summary>
	/// Default height of the application display.
	/// </summary>
	uint32 nHeight = 720;

	/// <summary>
	/// The path to the C# scripting project DLL.
	/// This will be required if UseScripting is true.
	/// </summary>
	std::string pathScriptingDll;

	std::string pathAssetsRoot = "assets";

	/// <summary>
	/// Whether the application will use a C# scripting project.
	/// </summary>
	bool bUseScripting = false;

	/// <summary>
	/// Whether the game should pause its execution
	/// when the application window loses focus..
	/// </summary>
	bool bPauseOnFocusLost = false;

	bool bEnableVSync = true;
};

class ZApplication
{
public:
	ZApplication(SApplicationSpec spec);
	virtual ~ZApplication();

	void Run();
	void Close();

	void OnEvent(ZEvent& rEvent);

	void PushModule(ZModule* pModule);
	void PopModule(ZModule* pModule);
	void PushModuleOverlay(ZModule* pModule);

	inline ZWindow& GetWindow() { return *m_Window; }

	inline void SetImGuiEnabled(bool enabled) { m_bImGuiEnabled = enabled; }
	inline bool IsImGuiEnabled() const { return m_bImGuiEnabled; }

	inline ZModuleImGui* GetImGui() const { return m_pModuleImGui; }

	inline const std::string& GetName() const { return m_sName; }

	inline float GetFPS() { return m_fFPS; }
	inline float GetFrameTime() { return m_fTimestep; }

	inline void SetPaused(bool paused) { m_bPaused = paused; }
	inline bool IsPaused() const { return m_bPaused; }

	/// <returns>The Application singleton.</returns>
	inline static ZApplication* Get() { return s_pInstance; }

private:
	bool OnWindowClose(EvtWindowClose& rEvent);
	bool OnWindowResize(EvtWindowResize& rEvent);
	bool OnWindowFocus(EvtWindowFocus& rEvent);

private:
	std::unique_ptr<ZWindow> m_Window;
	ZModuleImGui* m_pModuleImGui;

	std::string m_sName;

	/// <summary>
	/// Whether the application is running.
	/// </summary>
	bool m_bRunning = true;

	/// <summary>
	/// Whether the game is running in a "freeze" state (might be true
	/// if the application window focus is lost).
	/// </summary>
	bool m_bPaused = false;

	/// <summary>
	/// Whether the application window is minimized.
	/// </summary>
	bool m_bMinimized = false;

	/// <summary>
	/// Whether the application window has focus.
	/// </summary>
	bool m_bFocused = true;

	bool m_bPauseOnFocusLost;

	bool m_bImGuiEnabled = true;

	ZModuleStack m_Modules;
	float32 m_fLastFrameTime = 0.0f;
		
	float32 m_fFPS = 0.0f;
	float32 m_fTimestep = 0.0f;

private:
	/// <summary>
	/// Application singleton.
	/// </summary>
	static ZApplication* s_pInstance;
};

/// <summary>
/// To be defined in the client code.
/// Used at start time  to create the actual client application instance.
/// </summary>
/// <returns>The client application.</returns>
ZApplication* CreateApplication();

}

