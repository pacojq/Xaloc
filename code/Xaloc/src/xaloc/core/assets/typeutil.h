#pragma once
#include "xaloc/types.h"
#include "xaloc/core/core.h"
#include "xaloc/core/assets/asset.h"
#include <string>
#include <vector>


namespace Xaloc
{

std::string AssetTypeToString(
	EAssetType eType);

EAssetType AssetTypeFromString(
	const std::string& sType);

EAssetType AssetTypeFromExtension(
	const std::string& sExtension);
	
}