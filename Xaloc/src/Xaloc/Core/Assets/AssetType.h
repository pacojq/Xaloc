#pragma once

namespace Xaloc {

	enum class AssetType
	{
		// Default, unknown file type
		File = 0,
		
		Text,
		Image,
		Audio,
		Shader,
		Material,
		Scene,
		Code,
		Dll,
		Font
	};

	class AssetTypes
	{
	public:
		static AssetType FromExtension(const std::string& extension);


	private:
		static inline std::vector<std::string> m_ImageFiles =
		{
			"png", "jpg"
		};

		static inline std::vector<std::string> m_AudioFiles =
		{
			"wav", "mp3", "ogg"
		};

		static inline std::vector<std::string> m_ShaderFiles =
		{
			"glsl", "hlsl"
		};
		
		static inline std::vector<std::string> m_CodeFiles = 
		{
			"cs", "cpp", "h", "hpp"
		};
	};
	
}