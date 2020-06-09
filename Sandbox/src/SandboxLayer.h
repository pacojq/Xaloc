#pragma once

#include "Xaloc.h"

#include <vector>

class SandboxLayer : public Xaloc::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Xaloc::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Xaloc::Event& e) override;

private:

	Xaloc::Ref<Xaloc::Scene> m_Scene;

	Xaloc::OrthographicCameraController m_CameraController;

	float m_TilingFactor;
	float m_Rotation;
	glm::vec4 m_FirstColor;
	glm::vec4 m_SecondColor;
	Xaloc::Ref<Xaloc::Texture2D> m_Texture;
	
	Xaloc::Ref<Xaloc::Framebuffer> m_Framebuffer;
	
	Xaloc::Entity m_Player;

	Xaloc::Ref<Xaloc::SubTexture2D> m_TileWater;
	std::unordered_map<char, Xaloc::Ref<Xaloc::SubTexture2D>> s_TextureMap;
	float m_TilesDepth = -0.1f;

};