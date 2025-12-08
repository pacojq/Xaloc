#pragma once
#include "tooling/imgui/views/console.h"
#include "tooling/imgui/views/profiler.h"
#include "xaloc/core/module.h"


namespace Xaloc
{

/*
	Specific layer for ImGui rendering.
	To be implemented by each rendering platform.
*/
class ZModuleImGui : public ZModule
{
public:
	static ZModuleImGui* Create();

	virtual void Begin() = 0;
	virtual void End() = 0;

	virtual void OnEvent(ZEvent& rEvent) override;

	void SetBlockEvents(bool bBlockEvents) { m_bBlockEvents = bBlockEvents; }

	const ZRef<ZImGuiProfiler>& GetProfiler() const { return m_pProfiler; }

	inline void ShowConsole() { m_bShowConsole = true; }
	inline void ShowProfiler() { m_bShowProfiler = true; }

protected:
	ZModuleImGui::ZModuleImGui();

	/// <summary>
	/// To be called by children on attach layer.
	/// </summary>
	void SetupImGui();

protected:
	ZRef<ZImGuiProfiler> m_pProfiler;

	bool m_bBlockEvents = false;
	bool m_bShowConsole = true;
	bool m_bShowProfiler = true; // TODO debug, change to false by default
};

}