#pragma once

#include "EditorCamera.h"
#include "EditorViewport.h"

#include "Xaloc.h"

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

		struct SelectedEntity
		{
			Entity Entity;
			// TODO cast a ray and get distance float Distance;
		};
		void OnEntitySelected(const SelectedEntity& selection);

		
	private:

		RuntimeLayer* m_RuntimeLayer = nullptr;
		bool m_OnRuntime = false;

		Ref<Scene> m_Scene;
		Scope<SceneHierarchyPanel> m_SceneHierarchyPanel;
		Scope<AssetManagerPanel> m_AssetManagerPanel;

		std::vector<SelectedEntity> m_SelectionContext;

		Entity m_MainCamera;
		
		Ref<EditorCamera> m_EditorCamera;
		Ref<RenderPass> m_EditorRenderPass;
		
		Ref<RenderPass> m_RenderPass;
		Ref<RenderPass> m_GuizmoRenderPass;
		
		Ref<EditorViewport> m_GameViewport;
		Ref<EditorViewport> m_SceneViewport;


	// Editor Windows
	private:
		bool m_ShowWindowConsole = true; // TODO
		bool m_ShowWindowRenderStats = true;
	};

}