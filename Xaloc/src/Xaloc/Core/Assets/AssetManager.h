#pragma once

#include "Xaloc/Files/FileInfo.h"

#include "Xaloc/Renderer/Shader.h"
#include "Xaloc/Renderer/Texture.h"
#include "Xaloc/Renderer/SubTexture2D.h"

namespace Xaloc {

	// TODO create a file manager, so the asset manager can work in multiple platforms

	using AssetId = std::string;
	
	class AssetManager
	{
	public:
		static void Init(const std::string& rootPath);

		static DirectoryInfo GetRoot();

		static DirectoryContent ReadDirectory(const DirectoryInfo& dir);

		static void LoadTexture(const AssetId& id, const std::string& path);
		static Ref<Texture2D> GetTexture(const AssetId& id);

	private:
		static std::string ParseFileName(const std::string& str, const char delim, std::vector<std::string>& out);
		static std::string ParseFileExtension(const std::string& filename);

		
	private:
		static inline std::string s_RootPath = "";
		
	};

}