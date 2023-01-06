#pragma once

#include "Xaloc.h"

#include <vector>

using namespace Xaloc;

class SandboxLayer : public Xaloc::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Event& e) override;

private:

	Xaloc::Ref<Scene> m_Scene;

	float m_TilingFactor;
	float m_Rotation;
	glm::vec4 m_FirstColor;
	glm::vec4 m_SecondColor;
	Xaloc::Ref<Texture2D> m_Texture;
	
	Xaloc::Entity m_Player;

	Xaloc::Ref<SubTexture2D> m_TileWater;
	std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
	float m_TilesDepth = -0.1f;

};