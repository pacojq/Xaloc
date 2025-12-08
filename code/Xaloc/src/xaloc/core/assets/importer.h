#pragma once
#include "xaloc/core/assets/asset.h"
#include "xaloc/core/assets/metadata.h"

namespace Xaloc {
	class ZAssetSerializer;
}


namespace Xaloc
{

class ZAssetImporter
{
public:
	static void Init();

	static void Serialize(
		const SAssetMetadata& metadata,
		const ZRef<Asset>& asset);

	static void Serialize(
		const ZRef<Asset>& asset);

	static bool TryLoadData(
		const SAssetMetadata& metadata,
		ZRef<Asset>& asset);
		
	static bool GetInvalidAsset(EAssetType eType, ZRef<Asset>& asset);

private:
	// TODO: use an array, not a map
	static std::unordered_map<EAssetType, ZScope<ZAssetSerializer>> s_Serializers;
};

}
