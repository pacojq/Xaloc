#pragma once

#include "Asset.h"

#include <filesystem>

namespace Xaloc {

	struct AssetMetadata
	{
		AssetHandle Handle = 0;
		AssetType Type;

		/// <summary>
		/// File path, relative to the assets root directory.
		/// </summary>
		std::filesystem::path FilePath;

		bool IsDataLoaded = false;
		bool IsMemoryAsset = false;

		bool IsValid() const { return Handle != 0 && !IsMemoryAsset; }
	};
}
