#include "xapch.h"
#include "AssetManager.h"

#include "AssetManagerFilesystem.h"

#include <filesystem>

namespace Xaloc {

	static AssetMetadata s_NullMetadata;
	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::s_LoadedAssets;


	void AssetManager::Init(const std::string& rootFolder)
	{
		s_RootFolder = rootFolder;

		bool isDir = std::filesystem::is_directory(rootFolder);
		XA_CORE_ASSERT(isDir, "Assets root path folder be a directory!");

		AssetImporter::Init();
		AssetManagerFilesystem::Init();
	}

	void AssetManager::Shutdown()
	{
		s_LoadedAssets.clear();
	}

	void AssetManager::LoadProjectAssets()
	{
		AssetManagerFilesystem::LoadAssets();

		// Process root directory looking for not loaded assets
		auto rootDir = GetRoot();
		ScanDirectory(rootDir);

		AssetManagerFilesystem::SaveAssets();
	}




	void AssetManager::ScanDirectory(const DirectoryInfo& dir)
	{
		XA_CORE_WARN("Scanning directory: '{}'", dir.AbsolutePath);

		auto content = ReadDirectory(dir);

		for (int i = 0; i < content.Files.size(); i++)
		{
			AssetHandle handle = AssetManager::GetAssetHandleFromFilePath(content.Files[i].AbsolutePath);
			if (handle) continue; // Existing asset

			AssetType type = AssetManager::GetAssetTypeFromPath(content.Files[i].AbsolutePath);
			if (type == AssetType::None)
				continue;

			AssetMetadata metadata;
			metadata.Handle = AssetHandle();
			metadata.FilePath = AssetManager::GetRelativePath(content.Files[i].AbsolutePath);
			metadata.Type = type;

			s_AssetRegistry[metadata.Handle] = metadata;

			XA_CORE_INFO("        New asset: '{}'", content.Files[i].AbsolutePath);
		}

		for (int i = 0; i < content.Directories.size(); i++)
		{
			ScanDirectory(content.Directories[i]);
		}
	}



	DirectoryInfo AssetManager::GetRoot()
	{
		std::filesystem::path path{ std::filesystem::path(s_RootFolder) };

		std::string name = ParseFileName(s_RootFolder, '\\', std::vector<std::string>());
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





	std::filesystem::path AssetManager::GetFileSystemPath(const AssetMetadata& metadata)
	{
		std::filesystem::path path{ std::filesystem::path(s_RootFolder / metadata.FilePath) };
		return path;
	}


	std::filesystem::path AssetManager::GetRelativePath(const std::filesystem::path& filepath)
	{
		std::filesystem::path root{ std::filesystem::path(s_RootFolder) };

		std::filesystem::path relativePath = filepath.lexically_normal();
		std::string temp = filepath.string();
		if (temp.find(root.string()) != std::string::npos)
		{
			relativePath = std::filesystem::relative(filepath, root.string());
			if (relativePath.empty())
			{
				relativePath = filepath.lexically_normal();
			}
		}
		return relativePath;
	}






	const AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
	{
		if (s_AssetRegistry.Contains(handle))
			return s_AssetRegistry[handle];

		return s_NullMetadata;
	}

	const AssetMetadata& AssetManager::GetMetadata(const std::filesystem::path& filepath)
	{
		const auto relativePath = GetRelativePath(filepath);

		for (auto& [handle, metadata] : s_AssetRegistry)
		{
			if (metadata.FilePath == relativePath)
				return metadata;
		}

		return s_NullMetadata;
	}

	AssetHandle AssetManager::GetAssetHandleFromFilePath(const std::filesystem::path& filepath)
	{
		return GetMetadata(filepath).Handle;
	}

	AssetMetadata& AssetManager::GetMetadataInternal(AssetHandle handle)
	{
		if (s_AssetRegistry.Contains(handle))
			return s_AssetRegistry[handle];

		XA_CORE_ERROR("Asset registry does not contain asset with ID '{0}' !", (uint16_t)handle);
		return s_NullMetadata; // make sure to check return value before you go changing it!
	}









	AssetHandle AssetManager::ImportAsset(const std::filesystem::path& filepath)
	{
		std::filesystem::path path = GetRelativePath(filepath);
		//XA_CORE_ERROR("Importing asset at '{0}'", path.string());

		if (auto& metadata = GetMetadata(path); metadata.IsValid())
		{
			//XA_CORE_ERROR("Asset was already imported!");
			return metadata.Handle;
		}

		AssetType type = GetAssetTypeFromPath(path);
		if (type == AssetType::None)
		{
			XA_CORE_ERROR("Importing asset of invalid type!");
			return 0;
		}

		AssetMetadata metadata;
		metadata.Handle = AssetHandle();
		metadata.FilePath = path;
		metadata.Type = type;
		s_AssetRegistry[metadata.Handle] = metadata;

		return metadata.Handle;
	}






	/*
	void AssetManager::LoadTexture(AssetHandle assetHandle, const std::string& path)
	{
		// TODO handle repeated IDs
		Ref<Texture2D> tex = Texture2D::Create(path);
		tex->AssetHandle = assetHandle;
		s_Data.Textures[assetHandle] = tex;
	}


	Ref<Texture2D> AssetManager::GetTexture(AssetHandle assetHandle)
	{
		// TODO handle errors and return pink texture
		return s_Data.Textures[assetHandle];
	}
	*/




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