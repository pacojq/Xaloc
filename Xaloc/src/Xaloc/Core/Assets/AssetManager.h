#pragma once

#include "Xaloc/Renderer/Shader.h"
#include "Xaloc/Renderer/Texture.h"
#include "Xaloc/Renderer/SubTexture2D.h"

namespace Xaloc {

	class AssetManager
	{
	public:

		static void LoadTexture(const std::string& id, const std::string& path);
		static Ref<Texture2D> GetTexture(const std::string& id);

	};

}