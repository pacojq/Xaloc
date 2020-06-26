#pragma once

#include "Xaloc.h"

#include "Xaloc/Editor/SceneHierarchyPanel.h"

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

	private:

		struct SelectedEntity
		{
			Entity Entity;
			// TODO cast a ray and get distance float Distance;
		};
		void OnEntitySelected(const SelectedEntity& selection);
		
		std::pair<float, float> GetMouseViewportSpace();

		
	private:

		Ref<Scene> m_Scene;
		Scope<SceneHierarchyPanel> m_SceneHierarchyPanel;

		std::vector<SelectedEntity> m_SelectionContext;

		OrthographicCameraController m_CameraController;

		float m_TilingFactor;
		float m_Rotation;
		glm::vec4 m_FirstColor;
		glm::vec4 m_SecondColor;
		Ref<Texture2D> m_Texture;

		//Ref<Framebuffer> m_Framebuffer;
		Ref<RenderPass> m_RenderPass;
		Ref<RenderPass> m_GuizmoRenderPass;
		
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];
		

		Entity m_Player;

		Ref<SubTexture2D> m_TileWater;
		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
		float m_TilesDepth = -0.1f;


	// Editor Windows
	private:
		bool m_ShowWindowConsole = true; // TODO
		bool m_ShowWindowRenderStats = true;
	};

}