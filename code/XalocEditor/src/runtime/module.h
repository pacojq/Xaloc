#pragma once
#include "editor/viewports/game.h"
#include "panels/scenehierarchy/panel.h"
#include "xaloc/core/module.h"
#include "xaloc/events/application.h"
#include "xaloc/renderer/pipeline/framebuffer.h"
#include "xaloc/renderer/pipeline/renderpass.h"

namespace Xaloc {
	class ZFramebuffer;
	class ZRenderPass;
}

using namespace Xaloc;


namespace Editor
{

class ZModuleEditorRuntime : public ZModule
{
public:
	ZModuleEditorRuntime(const ZRef<ZScene>& scene);
	virtual ~ZModuleEditorRuntime() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(ZTimestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(ZEvent& rEvent) override;

	bool OnWindowResized(EvtWindowResize& rEvent);

private:
	ZRef<ZScene> m_Scene;
	ZFramebuffer* m_pFramebuffer;
	ZRenderPass* m_pRenderPass;
	ZRef<ZEditorGamePreview> m_GamePreview;

// Editor Windows
private:
	bool m_ShowWindowConsole = true; // TODO
	bool m_ShowWindowRenderStats = true;
};

}