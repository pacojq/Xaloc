#pragma once

#include "Asset.h"
#include "AssetMetadata.h"
#include "AssetRegistry.h"
#include "AssetImporter.h"

#include "Xaloc/Files/Filesystem.h"
#include "Xaloc/Files/FileInfo.h"

#include "Xaloc/Renderer/Shader.h"
#include "Xaloc/Renderer/Texture.h"

namespace Xaloc {

	class AssetManagerFilesystem;

	class AssetManager
	{
	public:
		using AssetsChangeEventFn = std::function<void(const std::vector<FileSystemChangedEvent>&)>;

	public:
		static void Init(const std::string& rootFolder);
		static void Shutdown();

		static void LoadProjectAssets();


		static DirectoryInfo GetRoot();
		static DirectoryContent ReadDirectory(const DirectoryInfo& dir);

		static std::filesystem::path GetFileSystemPath(const AssetMetadata& metadata);
		static std::string GetFileSystemPathString(const AssetMetadata& metadata) { return GetFileSystemPath(metadata).string(); }
		static std::filesystem::path GetRelativePath(const std::filesystem::path& filepath);

		static const AssetMetadata& GetMetadata(AssetHandle handle);
		static const AssetMetadata& GetMetadata(const std::filesystem::path& filepath);
		inline static const AssetMetadata& GetMetadata(const Ref<Asset>& asset) { return GetMetadata(asset->AssetHandle); }

		static AssetHandle GetAssetHandleFromFilePath(const std::filesystem::path& filepath);

		inline static AssetType GetAssetTypeFromPath(const std::filesystem::path& path)
		{
			return AssetTypes::FromExtension(path.extension().string());
		}



		static AssetHandle ImportAsset(const std::filesystem::path& filepath);
		//static bool ReloadData(AssetHandle assetHandle);



		template<typename T>
		static Ref<T> GetAsset(AssetHandle assetHandle)
		{
			//XA_CORE_ERROR("Getting asset with ID '{0}'", (uint16_t)assetHandle);
			XA_PROFILE_FUNCTION();

			//if (IsMemoryAsset(assetHandle))
			//	return s_MemoryAssets[assetHandle].As<T>();

			auto& metadata = GetMetadataInternal(assetHandle);
			if (!metadata.IsValid())
			{
				Ref<Asset> invalid = nullptr;
				AssetImporter::GetInvalidAsset(T::GetStaticAssetType(), invalid);
				return CastRef<Asset, T>(invalid);
			}

			Ref<Asset> asset = nullptr;
			if (!metadata.IsDataLoaded)
			{
				metadata.IsDataLoaded = AssetImporter::TryLoadData(metadata, asset);
				if (!metadata.IsDataLoaded)
					return nullptr; // TODO: default asset ?

				s_LoadedAssets[assetHandle] = asset;
			}
			else
			{
				asset = s_LoadedAssets[assetHandle];
			}

			return CastRef<Asset, T>(asset);
		}

		template<typename T>
		static Ref<T> GetAsset(const std::string& filepath)
		{
			return GetAsset<T>(GetAssetHandleFromFilePath(filepath));
		}




		//static void LoadTexture(AssetHandle assetHandle, const std::string& path);
		//static Ref<Texture2D> GetTexture(AssetHandle assetHandle);


	private:
		static std::string ParseFileName(const std::string& str, const char delim, std::vector<std::string>& out);
		static std::string ParseFileExtension(const std::string& filename);

		static AssetMetadata& GetMetadataInternal(AssetHandle handle);

		/// <summary>
		/// Scan a directory in search of new assets that
		/// will be included to the project.
		/// </summary>
		static void ScanDirectory(const DirectoryInfo& dir);


	private:
		static inline std::string s_RootFolder = "";
		inline static AssetRegistry s_AssetRegistry;

		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;


		friend class AssetManagerFilesystem;
	};

}