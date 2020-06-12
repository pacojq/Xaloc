#pragma once

#include "Xaloc.h"

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

	private:

		Ref<Scene> m_Scene;

		OrthographicCameraController m_CameraController;

		float m_TilingFactor;
		float m_Rotation;
		glm::vec4 m_FirstColor;
		glm::vec4 m_SecondColor;
		Ref<Texture2D> m_Texture;

		Ref<Framebuffer> m_Framebuffer;

		Entity m_Player;

		Ref<SubTexture2D> m_TileWater;
		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
		float m_TilesDepth = -0.1f;

	};

}