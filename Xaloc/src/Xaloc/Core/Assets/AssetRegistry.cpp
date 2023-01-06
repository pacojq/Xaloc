#include "xapch.h"
#include "AssetRegistry.h"

namespace Xaloc {

#define MAG_ASSETREGISTRY_LOG 0
#if MAG_ASSETREGISTRY_LOG
#define ASSET_LOG(...) XA_CORE_TRACE(__VA_ARGS__)
#else 
#define ASSET_LOG(...)
#endif

	AssetMetadata& AssetRegistry::operator[](const AssetHandle handle)
	{
		ASSET_LOG("Retrieving handle {}", handle);
		return m_AssetRegistry[handle];
	}

	const AssetMetadata& AssetRegistry::Get(const AssetHandle handle) const
	{
		XA_CORE_ASSERT(m_AssetRegistry.find(handle) != m_AssetRegistry.end());
		ASSET_LOG("Retrieving const handle {}", handle);
		return m_AssetRegistry.at(handle);
	}

	bool AssetRegistry::Contains(const AssetHandle handle) const
	{
		ASSET_LOG("Contains handle {}", handle);
		return m_AssetRegistry.find(handle) != m_AssetRegistry.end();
	}

	size_t AssetRegistry::Remove(const AssetHandle handle)
	{
		ASSET_LOG("Removing handle", handle);
		return m_AssetRegistry.erase(handle);
	}

	void AssetRegistry::Clear()
	{
		ASSET_LOG("Clearing registry");
		m_AssetRegistry.clear();
	}

}