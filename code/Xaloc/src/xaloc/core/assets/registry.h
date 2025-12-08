#pragma once
#include "xaloc/types.h"
#include "xaloc/core/assets/asset.h"
#include "xaloc/core/assets/metadata.h"
#include <unordered_map>


namespace Xaloc
{

class ZAssetRegistry
{
public:
	SAssetMetadata& operator[](ZAssetHandle handle);
	const SAssetMetadata& Get(ZAssetHandle handle) const;

	uint32 Count() const { return uint32(m_AssetRegistry.size()); }
	bool Contains(ZAssetHandle handle) const;
	uint32 Remove(ZAssetHandle handle);
	void Clear();

	auto begin() { return m_AssetRegistry.begin(); }
	auto end() { return m_AssetRegistry.end(); }
	auto begin() const { return m_AssetRegistry.cbegin(); }
	auto end() const { return m_AssetRegistry.cend(); }

private:
	std::unordered_map<ZAssetHandle, SAssetMetadata> m_AssetRegistry;
};

}
