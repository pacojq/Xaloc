#pragma once

#include "Xaloc/Core/Core.h"

#include <string>
#include <vector>

namespace Xaloc {

	enum class AssetFlag : uint16_t
	{
		None = 0,
		Missing = BIT(0),
		Invalid = BIT(1)
	};

	enum class AssetType
	{
		None = 0,

		Text,
		Texture2D,
		Sprite,
		Audio,
		Shader,
		Material,
		Animation,
		Scene,
		Code,
		Dll,
		Font,
		Prefab
	};

	class AssetTypes
	{
	public:
		static std::string TypeToString(AssetType assetType);
		static AssetType TypeFromString(const std::string& assetType);

		static AssetType FromExtension(const std::string& extension);


	private:
		static inline std::vector<std::string> m_ImageFiles =
		{
			".png", ".jpg"
		};

		static inline std::vector<std::string> m_AudioFiles =
		{
			".wav", ".mp3", ".ogg"
		};

		static inline std::vector<std::string> m_ShaderFiles =
		{
			".glsl", ".hlsl"
		};
		
		static inline std::vector<std::string> m_CodeFiles = 
		{
			".cs", ".cpp", ".h", ".hpp"
		};
	};
	
}