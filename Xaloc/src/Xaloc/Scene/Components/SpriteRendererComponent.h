#pragma once

#include "Xaloc/Renderer/SubTexture2D.h"

namespace Xaloc {

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<SubTexture2D> SubTexture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent(Ref<SubTexture2D>& subTexture)
			: SubTexture(subTexture)
		{

		}
	};

}