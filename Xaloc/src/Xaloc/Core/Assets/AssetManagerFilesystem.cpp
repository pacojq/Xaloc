#include "xapch.h"
#include "AssetManagerFilesystem.h"

#include "Xaloc/Core/Assets/Asset.h"
#include "Xaloc/Core/Assets/AssetType.h"


namespace Xaloc {

	AssetManager::AssetsChangeEventFn AssetManagerFilesystem::s_AssetsChangeCallback;

	void AssetManagerFilesystem::Init()
	{
		// TODO: 
		FileSystem::AddFileSystemChangedCallback(AssetManagerFilesystem::OnFileSystemChanged);
	}


	void AssetManagerFilesystem::OnFileSystemChanged(const std::vector<FileSystemChangedEvent>& events)
	{
		// Process all events before the refreshing the Content Browser
		for (const auto& e : events)
		{
			if (e.IsDirectory)
				continue; // TODO?

			switch (e.Action)
			{
				case FileSystemAction::Modified:
				{
					AssetHandle handle = AssetManager::GetAssetHandleFromFilePath(e.FilePath);
					const auto& metadata = AssetManager::GetMetadata(handle);

					if (metadata.IsValid())
					{
						//TODO: ReloadData(handle);
					}
					break;
				}
				case FileSystemAction::Rename:
				{
					AssetType previousType = AssetManager::GetAssetTypeFromPath(e.OldName);
					AssetType newType = AssetManager::GetAssetTypeFromPath(e.FilePath);

					if (previousType == AssetType::None && newType != AssetType::None)
					{
						AssetManager::ImportAsset(e.FilePath);
					}
					else
					{
						// TODO: OnAssetRenamed(GetAssetHandleFromFilePath(e.FilePath.parent_path() / e.OldName), e.FilePath);
					}
					break;
				}
				case FileSystemAction::Delete:
				{
					// TODO: OnAssetDeleted(GetAssetHandleFromFilePath(e.FilePath));
					break;
				}
			}
		}

		s_AssetsChangeCallback(events);
	}





	void AssetManagerFilesystem::LoadAssets()
	{
		std::filesystem::path filename = AssetManager::GetRoot().AbsolutePath;
		filename = filename / "assets.xalocdb";

		XA_CORE_ERROR("Loading assets database: {0}", filename.string());

		pugi::xml_document doc;

		std::ifstream stream(filename);
		pugi::xml_parse_result result = doc.load(stream);

		XA_CORE_TRACE("Asset Database read file result: {}", result.description());

		if (result.status != pugi::xml_parse_status::status_ok)
		{
			if (result.status == pugi::xml_parse_status::status_file_not_found)
			{
				XA_CORE_ASSERT(false, "Could not load assets database. File not found.");
			}
			else
			{
				XA_CORE_ASSERT(false, "Could not load assets database.");
			}
		}
	
		AssetManagerFilesystem::DeserializeAssets(doc);
	}

	void AssetManagerFilesystem::DeserializeAssets(pugi::xml_document& dbDoc)
	{
		pugi::xml_node root = dbDoc.child("assetdb");

		pugi::xml_node assetsRoot = root.child("assets");
		for (pugi::xml_node asset = assetsRoot.last_child(); asset; asset = asset.previous_sibling())
		{
			DeserializeAsset(asset);
		}

		/*
		<?xml version="1.0" encoding="UTF-8"?>
		<assetdb>

			<assets>
				<asset handle="0" type="Texture2D" path="assets/game/textures/tilemap.png"/>
			</assets>
  
		</assetdb>
		*/
	}


	static uint64_t ParseUint64(const std::string& str)
	{
		errno = 0;
		unsigned long long result = strtoull(str.c_str(), NULL, 16);

		if (errno == EINVAL)
		{
			XA_CORE_ASSERT(false, "Not a valid number!");
			return 0;
		}
		else if (errno == ERANGE)
		{
			XA_CORE_ASSERT(false, "Does not fit in an unsigned long long!");
			return 0;
		}

		return result;
	}

	void AssetManagerFilesystem::DeserializeAsset(pugi::xml_node& assetNode)
	{
		std::string strId = assetNode.attribute("handle").value();
		std::string strType = assetNode.attribute("type").value();
		std::string strPath = assetNode.attribute("path").value();

		XA_CORE_TRACE("    Loading asset. Handle='{}' Type='{}' Path='{}'", strId, strType, strPath);

		AssetMetadata metadata;
		metadata.Handle = (uint64_t)ParseUint64(strId);
		metadata.FilePath = AssetManager::GetRelativePath(strPath);
		metadata.Type = AssetTypes::TypeFromString(strType);

		if (metadata.Type == AssetType::None)
		{
			XA_CORE_WARN("    Could not import asset. Invalid type");
			return;
		}

		if (metadata.Type != AssetManager::GetAssetTypeFromPath(strPath))
		{
			XA_CORE_WARN("    Mismatch between stored AssetType and extension!");
			metadata.Type = AssetManager::GetAssetTypeFromPath(strPath);
		}

		if (!FileSystem::Exists(AssetManager::GetFileSystemPath(metadata)))
		{
			XA_CORE_WARN("    Asset file could not be found!");
			// TODO: try to find asset in file system
			return;
		}

		if (metadata.Handle == 0)
		{
			XA_CORE_WARN("    Invalid asset handle.");
			return;
		}

		AssetManager::s_AssetRegistry[metadata.Handle] = metadata;
	}






	void AssetManagerFilesystem::SaveAssets()
	{
		std::filesystem::path filename = AssetManager::GetRoot().AbsolutePath;
		filename = filename / "assets.xalocdb";

		XA_CORE_ERROR("Saving assets database: {0}", filename.string());

		pugi::xml_document doc = SerializeAssets();
		bool success = doc.save_file(filename.c_str());

		if (!success)
		{
			XA_CORE_ERROR("Could not save assets database!");
		}
		else XA_CORE_INFO("Assets database saved", filename);
	}

	pugi::xml_document AssetManagerFilesystem::SerializeAssets()
	{
		pugi::xml_document doc;
		pugi::xml_parse_result res = doc.load_string("<!-- Xaloc Engine. Assets database -->", pugi::parse_default | pugi::parse_comments);

		if (!res && res.status != pugi::status_no_document_element)
		{
			XA_CORE_ERROR("Parsing status: {}", res.status);
			XA_CORE_ERROR("Parsing error: {}", res.description());
			XA_CORE_ASSERT(false, "Something went wrong!");
		}

		pugi::xml_node root = doc.append_child("assetdb");
		pugi::xml_node assets = root.append_child("assets");

		for (auto pair : AssetManager::s_AssetRegistry)
		{
			auto& metadata = pair.second;
			SerializeAsset(doc, assets, metadata);
		}

		return doc;
	}



	static std::string Uint64ToString(uint64_t n)
	{
		char buff[16 + 1];
		snprintf(buff, sizeof(buff), "%llx", n);
		std::string result = buff;
		return result;
	}

	void AssetManagerFilesystem::SerializeAsset(pugi::xml_document& dbDoc, pugi::xml_node& assetsRoot, const AssetMetadata& metadata)
	{
		pugi::xml_node node = assetsRoot.append_child("asset");

		auto attrHandle = node.append_attribute("handle");
		attrHandle.set_value(Uint64ToString(metadata.Handle).c_str());

		auto attrType = node.append_attribute("type");
		attrType.set_value(AssetTypes::TypeToString(metadata.Type).c_str());

		auto attrPath = node.append_attribute("path");
		attrPath.set_value(metadata.FilePath.string().c_str());
	}

}