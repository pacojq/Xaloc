#pragma once
#include "xaloc/core/module.h"
#include "xaloc/scene/entity.h"
#include "xaloc/scene/scene.h"
#include "xaloc/renderer/resource/texture.h"
#include "xaloc/renderer/resource/subtexture.h"
#include <glm/glm.hpp>
#include <vector>

using namespace Xaloc;


class ZModuleSandbox : public ZModule
{
public:
	ZModuleSandbox();
	virtual ~ZModuleSandbox() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(ZTimestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(ZEvent& rEvent) override;

private:

	ZRef<ZScene> m_Scene;

	float m_TilingFactor;
	float m_Rotation;
	glm::vec4 m_FirstColor;
	glm::vec4 m_SecondColor;
	Xaloc::ZRef<ZTexture2D> m_Texture;
	
	ZEntity m_Player;

	ZRef<ZSubTexture2D> m_TileWater;
	std::unordered_map<char, ZRef<ZSubTexture2D>> s_TextureMap;
	float m_TilesDepth = -0.1f;

};