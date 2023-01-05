#pragma once

#include "Xaloc/Core/Assets/Asset.h"

namespace Xaloc {

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		AssetHandle Sprite;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const AssetHandle& sprite)
			: Sprite(sprite)
		{

		}
	};

}