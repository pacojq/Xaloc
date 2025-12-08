#pragma once
#include "editor/icons.h"
#include "editor/menubar.h"
#include "editor/viewports/game.h"
#include "editor/viewports/scene.h"
#include "panels/contentbrowser/panel.h"
#include "panels/scenehierarchy/panel.h"
#include "runtime/module.h"
#include "xaloc/core/module.h"
#include "xaloc/renderer/pipeline/framebuffer.h"
#include "xaloc/renderer/pipeline/renderpass.h"
#include "xaloc/renderer/cameras/editor.h"
#include "xaloc/renderer/resource/shader.h"
#include "imgui/imgui.h"

using namespace Xaloc;


namespace Editor
{

class ZModuleEditor : public ZModule
{
public:
	ZModuleEditor();
	virtual ~ZModuleEditor() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(ZTimestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(ZEvent& rEvent) override;

	bool OnMouseButtonPressed(EvtMouseButtonPressed& e);
	bool OnWindowResized(EvtWindowResize& e);

public:
	void OpenScene(const ZRef<ZScene>& scene);

	const ZRef<ZEditorIcons>& GetIcons() const { return m_Icons; }

private:
	void ResetDockSpace();

private:
	ZRef<ZEditorIcons> m_Icons;

	ZRef<ZEditorMenuBar> m_MenuBar;
	friend class ZEditorMenuBar;

	// TOOD: actually use this module
	ZModuleEditorRuntime* m_pRuntime = nullptr;
	bool m_bOnRuntime = false;

	ZRef<ZScene> m_Scene;

	ZEntity m_MainCamera;
		
	// TODO: main camera
	//InstanceId m_MainCamera; // TODO: change to "camera index" in a camera stack

	ZRef<ZEditorCamera> m_EditorCamera;

	ZFramebuffer* m_pFramebufferEditor;
	ZRenderPass* m_pRenderPassEditor;
	ZFramebuffer* m_pFramebufferPreview;
	ZRenderPass* m_pRenderPassPreview;

	ZRef<ZEditorGamePreview> m_GamePreview;
	ZRef<ZEditorSceneView> m_SceneViewport;

	ZRef<ZShader> m_CheckerboardShader;

private: // =============== Editor Windows ===============

	bool m_PanelsInitialized = false;
	ZScope<ZPanelSceneHierarchy> m_SceneHierarchyPanel;
	ZScope<ZPanelContentBrowser> m_ContentBrowserPanel;

	ImGuiID m_DockspaceId;
	bool m_DockspaceReady = false;

	bool m_ShowWindowConsole = true; // TODO
	bool m_ShowWindowRenderStats = false;
};

}