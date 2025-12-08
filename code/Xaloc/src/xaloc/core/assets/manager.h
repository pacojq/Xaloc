#pragma once
#include "xaloc/core/assets/asset.h"
#include "xaloc/core/assets/metadata.h"
#include "xaloc/core/assets/registry.h"
#include "xaloc/core/assets/importer.h"
#include "xaloc/filesystem/core.h"
#include "xaloc/filesystem/info.h"


namespace Xaloc
{

class ZAssetManager
{
public:
	static void Init(
		const std::string& pathRootFolder);

	static void Shutdown();

	static void LoadProjectAssets();

	static SDirectoryInfo GetRoot();

	static SDirectoryContent ReadDirectory(
		const SDirectoryInfo& directory);

	static std::filesystem::path GetFileSystemPath(
		const SAssetMetadata& metadata);

	static std::string GetFileSystemPathString(
		const SAssetMetadata& metadata)
	{
		return GetFileSystemPath(metadata).string();
	}

	static std::filesystem::path GetRelativePath(const std::filesystem::path& filepath);

	static const SAssetMetadata& ReadMetadata(
		ZAssetHandle handle);

	static const SAssetMetadata& ReadMetadata(
		const std::filesystem::path& filepath);

	inline static const SAssetMetadata& ReadMetadata(
		const ZRef<Asset>& asset)
	{
		return ReadMetadata(asset->AssetHandle);
	}

	static SAssetMetadata& WriteMetadata(ZAssetHandle handle);

	static ZAssetHandle GetAssetHandleFromFilePath(
		const std::filesystem::path& filepath);

	static EAssetType GetAssetTypeFromPath(
		const std::filesystem::path& path);

	static ZAssetHandle ImportAsset(
		const std::filesystem::path& filepath);

	template<typename T>
	static ZRef<T> GetAsset(ZAssetHandle assetHandle);

	template<typename T>
	static inline ZRef<T> GetAsset(const std::string& filepath)
	{
		return GetAsset<T>(GetAssetHandleFromFilePath(filepath));
	}

private:
	static inline std::string s_RootFolder = "";
	inline static ZAssetRegistry s_AssetRegistry;

	static std::unordered_map<ZAssetHandle, ZRef<Asset>> s_LoadedAssets;
};

template<typename T>
ZRef<T> ZAssetManager::GetAsset(ZAssetHandle assetHandle)
{
	//XA_CORE_ERROR("Getting asset with ID '{0}'", (uint16_t)assetHandle);
	XA_PROFILE_FUNCTION();

	auto& metadata = WriteMetadata(assetHandle);
	if (!metadata.IsValid())
	{
		ZRef<Asset> invalid = nullptr;
		ZAssetImporter::GetInvalidAsset(T::GetStaticAssetType(), invalid);
		return CastRef<Asset, T>(invalid);
	}

	ZRef<Asset> asset = nullptr;
	if (!metadata.bDataLoaded)
	{
		metadata.bDataLoaded = ZAssetImporter::TryLoadData(metadata, asset);
		if (!metadata.bDataLoaded)
		{
			return nullptr; // TODO: default asset ?
		}
		s_LoadedAssets[assetHandle] = asset;
	}
	else
	{
		asset = s_LoadedAssets[assetHandle];
	}
	return CastRef<Asset, T>(asset);
}

}