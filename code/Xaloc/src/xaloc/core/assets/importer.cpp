#include "xapch.h"
#include "xaloc/core/assets/importer.h"
#include "xaloc/core/assets/manager.h"
#include "xaloc/core/assets/serializers.h"


namespace Xaloc
{

void ZAssetImporter::Init()
{
	s_Serializers.clear();
	s_Serializers[EAssetType::Prefab] = CreateScope<ZAssetSerializerPrefab>();
	s_Serializers[EAssetType::Texture2D] = CreateScope<ZAssetSerializerTexture>();
	s_Serializers[EAssetType::Sprite] = CreateScope<ZAssetSerializerSprite>();
	//s_Serializers[AssetType::Material] = CreateScope<MaterialAssetSerializer>();
	//s_Serializers[AssetType::Audio] = CreateScope<AudioFileSourceSerializer>();
	//s_Serializers[AssetType::Room] = CreateScope<RoomAssetSerializer>();
	//s_Serializers[AssetType::Font] = CreateScope<FontSerializer>();
	//s_Serializers[AssetType::Animation] = CreateScope<AnimationAssetSerializer>();
	//s_Serializers[AssetType::Script] = CreateScope<ScriptFileSerializer>();
}

void ZAssetImporter::Serialize(
	const SAssetMetadata& metadata,
	const ZRef<Asset>& asset)
{
	if (s_Serializers.find(metadata.eType) == s_Serializers.end())
	{
		XA_CORE_WARN("There's currently no importer for assets of type {0}", metadata.pathResource.stem().string());
		return;
	}

	s_Serializers[asset->GetAssetType()]->Serialize(metadata, asset);
}

void ZAssetImporter::Serialize(const ZRef<Asset>& asset)
{
	const SAssetMetadata& metadata = ZAssetManager::ReadMetadata(asset->AssetHandle);
	Serialize(metadata, asset);
}

bool ZAssetImporter::TryLoadData(const SAssetMetadata& metadata, ZRef<Asset>& asset)
{
	if (s_Serializers.find(metadata.eType) == s_Serializers.end())
	{
		XA_CORE_ERROR("There's currently no importer for assets of type {0}", metadata.pathResource.stem().string());
		return false;
	}

	return s_Serializers[metadata.eType]->TryLoadData(asset, metadata);
}

bool ZAssetImporter::GetInvalidAsset(EAssetType eType, ZRef<Asset>& asset)
{
	if (s_Serializers.find(eType) == s_Serializers.end())
	{
		XA_CORE_ERROR("There's currently no importer for assets of type");
		return false;
	}
	return s_Serializers[eType]->GetInvalidAsset(asset);
}

std::unordered_map<EAssetType, ZScope<ZAssetSerializer>> ZAssetImporter::s_Serializers;

}
