#pragma once
#include "xaloc/core/assets/registry.h"
#include "xaloc/filesystem/core.h"
#include <pugixml.hpp>
#include <functional>
#include <vector>

namespace Xaloc {
	struct EvtFileChanged;
}


namespace Xaloc
{

using FnAssetFileChangedCallback = std::function<void(const std::vector<EvtFileChanged>&)>;

class ZAssetManagerFilesystem
{
public:
	static void Init();

	static void LoadAssets(ZAssetRegistry& rAssetRegistry);
	static void SaveAssets(const ZAssetRegistry& assetRegistry);

private:
	static void OnFileSystemChanged(
		const std::vector<EvtFileChanged>& events);

private:
	static FnAssetFileChangedCallback s_assetsChangeCallback;
};
}