#pragma once

#include "Xaloc.h"

#include "EditorIcons.h"
#include "EditorMenuBar.h"
#include "Viewports/EditorGamePreview.h"
#include "Viewports/EditorSceneView.h"

#include "Xaloc/Renderer/Cameras/EditorCamera.h"
#include "Xaloc/Renderer/Cameras/EditorCameraPerspective.h"
#include "Xaloc/Renderer/Cameras/EditorCameraOrthographic.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowser/ContentBrowserPanel.h"

#include "Layers/RuntimeLayer.h"

#include "imgui/imgui.h"

namespace Xaloc {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
		
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	public:
		void OpenScene(const Ref<Scene>& scene);

		const Ref<EditorIcons>& GetIcons() const { return m_Icons; }


	private:
		void ResetDockSpace();

		
	private:
		Ref<EditorIcons> m_Icons;

		Ref<EditorMenuBar> m_MenuBar;
		friend class EditorMenuBar;

		RuntimeLayer* m_RuntimeLayer = nullptr;
		bool m_OnRuntime = false;

		Ref<Scene> m_Scene;

		Entity m_MainCamera;
		
		// TODO: main camera
		//InstanceId m_MainCamera; // TODO: change to "camera index" in a camera stack

		Ref<EditorCamera> m_EditorCamera; // The currently active editor camera
		Ref<EditorCameraPerspective> m_EditorCameraPerspective;
		Ref<EditorCameraOrthographic> m_EditorCameraOrthographic;


		Ref<RenderPass> m_EditorRenderPass;
		Ref<RenderPass> m_PreviewRenderPass;

		Ref<EditorGamePreview> m_GamePreview;
		Ref<EditorSceneView> m_SceneViewport;

		Ref<Shader> m_CheckerboardShader;
		

	private: // =============== Editor Windows ===============

		bool m_PanelsInitialized = false;
		Scope<SceneHierarchyPanel> m_SceneHierarchyPanel;
		Scope<ContentBrowserPanel> m_ContentBrowserPanel;

		ImGuiID m_DockspaceId;
		bool m_DockspaceReady = false;

		bool m_ShowWindowConsole = true; // TODO
		bool m_ShowWindowRenderStats = false;
	};

}