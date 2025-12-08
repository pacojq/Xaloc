#pragma once
#include "xaloc/core/assets/asset.h"
#include <filesystem>


namespace Xaloc
{

struct SAssetMetadata
{
	ZAssetHandle nHandle = 0;
	EAssetType eType;

	/// <summary>
	/// File path, relative to the assets root directory.
	/// </summary>
	std::filesystem::path pathResource;

	bool bDataLoaded = false;
	bool bMemoryAsset = false;

	bool IsValid() const { return nHandle != 0 && !bMemoryAsset; }
};

}
