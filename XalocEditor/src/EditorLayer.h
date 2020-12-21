#pragma once

#include "EditorViewport.h"

#include "Xaloc.h"

#include "Xaloc/Renderer/EditorCamera.h"

#include "Panels/AssetManagerPanel.h"
#include "Panels/SceneHierarchyPanel.h"

#include "Layers/RuntimeLayer.h"

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


		static inline std::string s_FilenameFilter = "Xaloc Scene\0*.xaloc\0 All files\0*.*\0";
		

	// Editor Windows
	private:
		bool m_ShowWindowConsole = true; // TODO
		bool m_ShowWindowRenderStats = true;
	};

}