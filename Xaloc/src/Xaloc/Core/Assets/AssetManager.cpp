#include "xapch.h"
#include "AssetManager.h"

#include <filesystem>

namespace Xaloc {

	struct AssetData
	{
		std::unordered_map<std::string, Ref<Texture2D>> Textures;
	};
	static AssetData s_Data;


	void AssetManager::Init(const std::string& rootPath)
	{
		s_RootPath = rootPath;

		bool isDir = std::filesystem::is_directory(rootPath);
		XA_CORE_ASSERT(isDir, "Assets root path must be a directory!");
	}

	DirectoryInfo AssetManager::GetRoot()
	{
		std::filesystem::path path{ std::filesystem::path(s_RootPath) };

		std::string name = ParseFileName(s_RootPath, '\\', std::vector<std::string>());
		std::string abs = std::filesystem::absolute(path).string(); // u8string();

		DirectoryInfo result;
		result.Name = name;
		result.AbsolutePath = abs;
		
		return result;
	}


	DirectoryContent AssetManager::ReadDirectory(const DirectoryInfo& dir)
	{
		DirectoryContent result;

		for (const auto& entry : std::filesystem::directory_iterator(dir.AbsolutePath))
		{
			std::string name = ParseFileName(entry.path().string(), '\\', std::vector<std::string>());
			std::string abs = entry.path().string();
			
			if (std::filesystem::is_directory(entry))
			{
				DirectoryInfo d = { name, abs };
				result.Directories.push_back(d);
			}
			else
			{
				std::string ext = ParseFileExtension(entry.path().string());
				FileInfo f(name, ext, abs);
				result.Files.push_back(f);
			}
		}

		return result;
	}


	
	

	void AssetManager::LoadTexture(const AssetId& id, const std::string& path)
	{
		// TODO handle repeated IDs
		Ref<Texture2D> tex = Texture2D::Create(path);
		tex->m_AssetID = id;
		s_Data.Textures[id] = tex;
	}


	Ref<Texture2D> AssetManager::GetTexture(const AssetId& id)
	{
		// TODO handle errors and return pink texture
		return s_Data.Textures[id];
	}








	std::string AssetManager::ParseFileName(const std::string& str, const char delim, std::vector<std::string>& out)
	{
		size_t start;
		size_t end = 0;

		while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
		{
			end = str.find(delim, start);
			out.push_back(str.substr(start, end - start));
		}

		return out[out.size() - 1];
	}

	std::string AssetManager::ParseFileExtension(const std::string& filename)
	{
		size_t start;
		size_t end = 0;
		std::vector<std::string> out;

		while ((start = filename.find_first_not_of(".", end)) != std::string::npos)
		{
			end = filename.find(".", start);
			out.push_back(filename.substr(start, end - start));
		}

		return out[out.size() - 1];
	}

}