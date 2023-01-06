#include "xapch.h"
#include "AssetImporter.h"
#include "AssetManager.h"

//#include "Hazel/Animation/AnimationAssetSerializer.h"

namespace Xaloc {

	void AssetImporter::Init()
	{
		s_Serializers.clear();
		s_Serializers[AssetType::Prefab] = CreateScope<PrefabSerializer>();
		s_Serializers[AssetType::Texture2D] = CreateScope<TextureSerializer>();
		s_Serializers[AssetType::Sprite] = CreateScope<SpriteSerializer>();
		//s_Serializers[AssetType::Material] = CreateScope<MaterialAssetSerializer>();
		//s_Serializers[AssetType::Audio] = CreateScope<AudioFileSourceSerializer>();
		//s_Serializers[AssetType::Room] = CreateScope<RoomAssetSerializer>();
		//s_Serializers[AssetType::Font] = CreateScope<FontSerializer>();
		//s_Serializers[AssetType::Animation] = CreateScope<AnimationAssetSerializer>();
		//s_Serializers[AssetType::Script] = CreateScope<ScriptFileSerializer>();
	}

	void AssetImporter::Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset)
	{
		if (s_Serializers.find(metadata.Type) == s_Serializers.end())
		{
			XA_CORE_WARN("There's currently no importer for assets of type {0}", metadata.FilePath.stem().string());
			return;
		}

		s_Serializers[asset->GetAssetType()]->Serialize(metadata, asset);
	}

	void AssetImporter::Serialize(const Ref<Asset>& asset)
	{
		const AssetMetadata& metadata = AssetManager::GetMetadata(asset->AssetHandle);
		Serialize(metadata, asset);
	}

	bool AssetImporter::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset)
	{
		if (s_Serializers.find(metadata.Type) == s_Serializers.end())
		{
			XA_CORE_ERROR("There's currently no importer for assets of type {0}", metadata.FilePath.stem().string());
			return false;
		}

		return s_Serializers[metadata.Type]->TryLoadData(metadata, asset);
	}


	bool AssetImporter::GetInvalidAsset(AssetType type, Ref<Asset>& asset)
	{
		if (s_Serializers.find(type) == s_Serializers.end())
		{
			XA_CORE_ERROR("There's currently no importer for assets of type");
			return false;
		}
		return s_Serializers[type]->GetInvalidAsset(asset);
	}


	std::unordered_map<AssetType, Scope<AssetSerializer>> AssetImporter::s_Serializers;

}
