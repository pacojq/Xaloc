#include "xapch.h"
#include "xaloc/core/assets/asset.h"
#include "xaloc/core/assets/manager.h"
#include "xaloc/core/assets/filesystem.h"
#include "xaloc/core/assets/typeutil.h"
#include "xaloc/debugging/assert.h"
#include <filesystem>


namespace Xaloc
{

/// <summary>
/// Scan a directory in search of new assets that
/// will be included to the project.
/// </summary>
static void ScanDirectory(
	ZAssetRegistry& rAssetRegistry,
	const SDirectoryInfo& directory)
{
	XA_CORE_WARN("Scanning directory: '{}'", directory.pathAbsolute);
	const SDirectoryContent content = ZAssetManager::ReadDirectory(directory);
	const uint32 nFileCount = uint32(content.aFiles.size());
	const uint32 nDirectoryCount = uint32(content.aDirectories.size());

	for (uint32 nFile = 0; nFile < nFileCount; ++nFile)
	{
		const SFileInfo file = content.aFiles[nFile];
		const ZAssetHandle handle = ZAssetManager::GetAssetHandleFromFilePath(
			file.pathAbsolute);
		if (handle)
		{
			continue; // Existing asset
		}
		const EAssetType eType = ZAssetManager::GetAssetTypeFromPath(file.pathAbsolute);
		if (eType == EAssetType::None)
		{
			continue;
		}
		SAssetMetadata metadata;
		metadata.nHandle = ZAssetHandle();
		metadata.pathResource = ZAssetManager::GetRelativePath(file.pathAbsolute);
		metadata.eType = eType;

		rAssetRegistry[metadata.nHandle] = metadata;

		XA_CORE_INFO("        New asset: '{}'", file.pathAbsolute);
	}

	for (uint32 nDirectory = 0; nDirectory < nDirectoryCount; ++nDirectory)
	{
		ScanDirectory(rAssetRegistry, content.aDirectories[nDirectory]);
	}
}

static std::string ParseFileName(
	const std::string& pathFile,
	const char cDelimiter)
{
	// TODO: this is a VERY crappy way of getting the file extension

	size_t start;
	size_t end = 0;
	std::vector<std::string> aSplit;

	while ((start = pathFile.find_first_not_of(cDelimiter, end)) != std::string::npos)
	{
		end = pathFile.find(cDelimiter, start);
		aSplit.push_back(pathFile.substr(start, end - start));
	}

	return aSplit[aSplit.size() - 1];
}

static std::string ParseFileExtension(const std::string& pathFile)
{
	// TODO: this is a VERY crappy way of getting the file extension

	size_t start;
	size_t end = 0;
	std::vector<std::string> aSplit;

	while ((start = pathFile.find_first_not_of(".", end)) != std::string::npos)
	{
		end = pathFile.find(".", start);
		aSplit.push_back(pathFile.substr(start, end - start));
	}

	return aSplit[aSplit.size() - 1];
}

static SAssetMetadata s_NullMetadata;
std::unordered_map<ZAssetHandle, ZRef<Asset>> ZAssetManager::s_LoadedAssets;

void ZAssetManager::Init(const std::string& rootFolder)
{
	s_RootFolder = rootFolder;

	bool isDir = std::filesystem::is_directory(rootFolder);
	XA_CORE_ASSERT(isDir, "Assets root path folder be a directory!");

	ZAssetImporter::Init();
	ZAssetManagerFilesystem::Init();
}

void ZAssetManager::Shutdown()
{
	s_LoadedAssets.clear();
}

void ZAssetManager::LoadProjectAssets()
{
	ZAssetManagerFilesystem::LoadAssets(s_AssetRegistry);

	// Process root directory looking for assets not yet loaded
	const SDirectoryInfo rootDir = GetRoot();
	ScanDirectory(s_AssetRegistry, rootDir);

	ZAssetManagerFilesystem::SaveAssets(s_AssetRegistry);
}

SDirectoryInfo ZAssetManager::GetRoot()
{
	std::filesystem::path path{ std::filesystem::path(s_RootFolder) };

	std::string name = ParseFileName(s_RootFolder, '\\');
	std::string abs = std::filesystem::absolute(path).string(); // u8string();

	SDirectoryInfo result;
	result.sName = name;
	result.pathAbsolute = abs;

	return result;
}

SDirectoryContent ZAssetManager::ReadDirectory(const SDirectoryInfo& dir)
{
	SDirectoryContent result;

	for (const auto& entry : std::filesystem::directory_iterator(dir.pathAbsolute))
	{
		std::string name = ParseFileName(entry.path().string(), '\\');
		std::string abs = entry.path().string();

		if (std::filesystem::is_directory(entry))
		{
			SDirectoryInfo d = { name, abs };
			result.aDirectories.push_back(d);
		}
		else
		{
			std::string ext = ParseFileExtension(entry.path().string());
			SFileInfo f(name, ext, abs);
			result.aFiles.push_back(f);
		}
	}

	return result;
}

std::filesystem::path ZAssetManager::GetFileSystemPath(const SAssetMetadata& metadata)
{
	std::filesystem::path path{ std::filesystem::path(s_RootFolder / metadata.pathResource) };
	return path;
}

std::filesystem::path ZAssetManager::GetRelativePath(const std::filesystem::path& filepath)
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

const SAssetMetadata& ZAssetManager::ReadMetadata(ZAssetHandle handle)
{
	if (s_AssetRegistry.Contains(handle))
	{
		return s_AssetRegistry[handle];
	}
	return s_NullMetadata;
}

const SAssetMetadata& ZAssetManager::ReadMetadata(const std::filesystem::path& filepath)
{
	const auto relativePath = GetRelativePath(filepath);

	for (auto& [handle, metadata] : s_AssetRegistry)
	{
		if (metadata.pathResource == relativePath)
		{
			return metadata;
		}
	}
	return s_NullMetadata;
}

ZAssetHandle ZAssetManager::GetAssetHandleFromFilePath(const std::filesystem::path& filepath)
{
	return ReadMetadata(filepath).nHandle;
}

EAssetType ZAssetManager::GetAssetTypeFromPath(const std::filesystem::path& path)
{
	return AssetTypeFromExtension(path.extension().string());
}

SAssetMetadata& ZAssetManager::WriteMetadata(ZAssetHandle handle)
{
	if (s_AssetRegistry.Contains(handle))
	{
		return s_AssetRegistry[handle];
	}
	XA_CORE_ERROR("Asset registry does not contain asset with ID '{0}' !", (uint16_t)handle);
	return s_NullMetadata; // make sure to check return value before you go changing it!
}

ZAssetHandle ZAssetManager::ImportAsset(const std::filesystem::path& filepath)
{
	std::filesystem::path path = GetRelativePath(filepath);
	//XA_CORE_ERROR("Importing asset at '{0}'", path.string());

	if (auto& metadata = ReadMetadata(path); metadata.IsValid())
	{
		//XA_CORE_ERROR("Asset was already imported!");
		return metadata.nHandle;
	}

	EAssetType eType = GetAssetTypeFromPath(path);
	if (eType == EAssetType::None)
	{
		XA_CORE_ERROR("Importing asset of invalid type!");
		return 0;
	}

	SAssetMetadata metadata;
	metadata.nHandle = ZAssetHandle();
	metadata.pathResource = path;
	metadata.eType = eType;
	s_AssetRegistry[metadata.nHandle] = metadata;

	return metadata.nHandle;
}

}