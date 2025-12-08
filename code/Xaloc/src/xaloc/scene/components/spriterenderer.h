#pragma once
#include "xaloc/core/assets/Asset.h"
#include <glm/glm.hpp>


namespace Xaloc
{

struct CmpSpriteRenderer
{
	glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	ZAssetHandle Sprite;
	float fTilingFactor = 1.0f;

	CmpSpriteRenderer() = default;
	CmpSpriteRenderer(const ZAssetHandle& sprite)
		: Sprite(sprite)
	{
	}
};

}