#pragma once

#include "AssetManager.h"

#include "Xaloc/Files/FileSystem.h"

#include <pugixml.hpp>

namespace Xaloc {

	class AssetManagerFilesystem
	{
	public:
		static void Init();

		static void LoadAssets();
		static void SaveAssets();


	private:
		static void OnFileSystemChanged(const std::vector<FileSystemChangedEvent>& events);

		static void DeserializeAssets(pugi::xml_document& dbDoc);
		static void DeserializeAsset(pugi::xml_node& assetNode);

		static pugi::xml_document SerializeAssets();
		static void SerializeAsset(pugi::xml_document& dbDoc, pugi::xml_node& assetsRoot, const AssetMetadata& metadata);


	private:
		static AssetManager::AssetsChangeEventFn s_AssetsChangeCallback;
	};
}