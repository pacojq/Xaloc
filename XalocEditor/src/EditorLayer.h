#pragma once

#include "Xaloc.h"

#include "EditorMenuBar.h"
#include "EditorViewport.h"

#include "Xaloc/Renderer/EditorCamera.h"

#include "Panels/AssetManagerPanel.h"
#include "Panels/SceneHierarchyPanel.h"

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

	private:
		void ResetDockSpace();

		
	private:
		Ref<EditorMenuBar> m_MenuBar;
		friend class EditorMenuBar;

		RuntimeLayer* m_RuntimeLayer = nullptr;
		bool m_OnRuntime = false;

		Ref<Scene> m_Scene;
		Scope<SceneHierarchyPanel> m_SceneHierarchyPanel;
		Scope<AssetManagerPanel> m_AssetManagerPanel;

		Entity m_MainCamera;
		
		EditorCamera m_EditorCamera;
		Ref<RenderPass> m_EditorRenderPass;
		
		Ref<RenderPass> m_RenderPass;
		Ref<RenderPass> m_GuizmoRenderPass;
		
		Ref<EditorViewport> m_GameViewport;
		Ref<EditorViewport> m_SceneViewport;
		

	// Editor Windows
	private:
		ImGuiID m_DockspaceId;
		bool m_DockspaceReady = false;

		bool m_ShowWindowConsole = true; // TODO
		bool m_ShowWindowRenderStats = false;
	};

}