#include "xapch.h"
#include "xaloc/core/assets/registry.h"
#include "xaloc/debugging/assert.h"


namespace Xaloc
{

#define MAG_ASSETREGISTRY_LOG 0
#if MAG_ASSETREGISTRY_LOG
#define ASSET_LOG(...) XA_CORE_TRACE(__VA_ARGS__)
#else 
#define ASSET_LOG(...)
#endif

SAssetMetadata& ZAssetRegistry::operator[](ZAssetHandle handle)
{
	ASSET_LOG("Retrieving handle {}", handle);
	return m_AssetRegistry[handle];
}

const SAssetMetadata& ZAssetRegistry::Get(ZAssetHandle handle) const
{
	XA_CORE_ASSERT(m_AssetRegistry.find(handle) != m_AssetRegistry.end());
	ASSET_LOG("Retrieving const handle {}", handle);
	return m_AssetRegistry.at(handle);
}

bool ZAssetRegistry::Contains(ZAssetHandle handle) const
{
	ASSET_LOG("Contains handle {}", handle);
	return m_AssetRegistry.find(handle) != m_AssetRegistry.end();
}

uint32 ZAssetRegistry::Remove(ZAssetHandle handle)
{
	ASSET_LOG("Removing handle", handle);
	return uint32(m_AssetRegistry.erase(handle));
}

void ZAssetRegistry::Clear()
{
	ASSET_LOG("Clearing registry");
	m_AssetRegistry.clear();
}

}