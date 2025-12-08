#include "xapch.h"
#include "xaloc/core/assets/asset.h"
#include "xaloc/core/assets/filesystem.h"
#include "xaloc/core/assets/manager.h"
#include "xaloc/core/assets/typeutil.h"
#include "xaloc/core/assets/util/parsing.h"
#include "xaloc/debugging/assert.h"
#include "xaloc/filesystem/watch.h"


namespace Xaloc
{

static void DeserializeAsset(
	ZAssetRegistry& rAssetRegistry,
	pugi::xml_node& assetNode)
{
	std::string strId = assetNode.attribute("handle").value();
	std::string strType = assetNode.attribute("type").value();
	std::string strPath = assetNode.attribute("path").value();

	XA_CORE_TRACE("    Loading asset. Handle='{}' Type='{}' Path='{}'", strId, strType, strPath);

	SAssetMetadata metadata;
	metadata.nHandle = (uint64_t)ParseUint64(strId);
	metadata.pathResource = ZAssetManager::GetRelativePath(strPath);
	metadata.eType = AssetTypeFromString(strType);

	if (metadata.eType == EAssetType::None)
	{
		XA_CORE_WARN("    Could not import asset. Invalid type");
		return;
	}

	if (metadata.eType != ZAssetManager::GetAssetTypeFromPath(strPath))
	{
		XA_CORE_WARN("    Mismatch between stored AssetType and extension!");
		metadata.eType = ZAssetManager::GetAssetTypeFromPath(strPath);
	}

	if (!FileExists(ZAssetManager::GetFileSystemPath(metadata)))
	{
		XA_CORE_WARN("    Asset file could not be found!");
		// TODO: try to find asset in file system
		return;
	}

	if (metadata.nHandle == 0)
	{
		XA_CORE_WARN("    Invalid asset handle.");
		return;
	}

	rAssetRegistry[metadata.nHandle] = metadata;
}

static void DeserializeAssets(
	ZAssetRegistry& rAssetRegistry,
	pugi::xml_document& dbDoc)
{
	pugi::xml_node root = dbDoc.child("assetdb");

	pugi::xml_node assetsRoot = root.child("assets");
	for (pugi::xml_node asset = assetsRoot.last_child(); asset; asset = asset.previous_sibling())
	{
		DeserializeAsset(rAssetRegistry, asset);
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

static void SerializeAsset(
	pugi::xml_document& dbDoc,
	pugi::xml_node& assetsRoot,
	const SAssetMetadata& metadata)
{
	pugi::xml_node node = assetsRoot.append_child("asset");

	auto attrHandle = node.append_attribute("handle");
	attrHandle.set_value(Uint64ToString(metadata.nHandle).c_str());

	auto attrType = node.append_attribute("type");
	attrType.set_value(AssetTypeToString(metadata.eType).c_str());

	auto attrPath = node.append_attribute("path");
	attrPath.set_value(metadata.pathResource.string().c_str());
}

static pugi::xml_document SerializeAssets(
	const ZAssetRegistry& assetRegistry)
{
	pugi::xml_document doc;
	pugi::xml_parse_result res = doc.load_string("<!-- Xaloc Engine. Assets database -->", pugi::parse_default | pugi::parse_comments);

	if (!res && res.status != pugi::status_no_document_element)
	{
		XA_CORE_ERROR("Parsing status: {}", (int)res.status);
		XA_CORE_ERROR("Parsing error: {}", res.description());
		XA_CORE_ASSERT(false, "Something went wrong!");
	}

	pugi::xml_node root = doc.append_child("assetdb");
	pugi::xml_node assets = root.append_child("assets");

	std::vector<ZAssetHandle> sorted;

	for (auto pair : assetRegistry)
	{
		sorted.push_back(pair.first);
	}
	std::sort(sorted.begin(), sorted.begin() + sorted.size());

	for (auto handle : sorted)
	{
		auto& metadata = assetRegistry.Get(handle);
		SerializeAsset(doc, assets, metadata);
	}

	return doc;
}

FnAssetFileChangedCallback ZAssetManagerFilesystem::s_assetsChangeCallback;

void ZAssetManagerFilesystem::Init()
{
	// TODO: 
	ZFileWatch::RegisterCallback(ZAssetManagerFilesystem::OnFileSystemChanged);
}

void ZAssetManagerFilesystem::OnFileSystemChanged(
	const std::vector<EvtFileChanged>& events)
{
	// Process all events before the refreshing the Content Browser
	for (const auto& e : events)
	{
		if (e.bIsDirectory)
		{
			continue; // TODO?
		}

		switch (e.eAction)
		{
		case EFileSystemAction::Modify:
		{
			ZAssetHandle handle = ZAssetManager::GetAssetHandleFromFilePath(e.path);
			const auto& metadata = ZAssetManager::ReadMetadata(handle);

			if (metadata.IsValid())
			{
				//TODO: ReloadData(handle);
			}
			break;
		}
		case EFileSystemAction::Rename:
		{
			const EAssetType eTypePrevious = ZAssetManager::GetAssetTypeFromPath(e.sNamePrevious);
			const EAssetType eTypeNew = ZAssetManager::GetAssetTypeFromPath(e.path);

			if (eTypePrevious == EAssetType::None && eTypeNew != EAssetType::None)
			{
				ZAssetManager::ImportAsset(e.path);
			}
			else
			{
				// TODO: OnAssetRenamed(GetAssetHandleFromFilePath(e.FilePath.parent_path() / e.OldName), e.FilePath);
			}
			break;
		}
		case EFileSystemAction::Delete:
		{
			// TODO: OnAssetDeleted(GetAssetHandleFromFilePath(e.FilePath));
			break;
		}
		}
	}

	s_assetsChangeCallback(events);
}

void ZAssetManagerFilesystem::LoadAssets(
	ZAssetRegistry& rAssetRegistry)
{
	std::filesystem::path filename = ZAssetManager::GetRoot().pathAbsolute;
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
	
	DeserializeAssets(rAssetRegistry, doc);
}

void ZAssetManagerFilesystem::SaveAssets(
	const ZAssetRegistry& assetRegistry)
{
	std::filesystem::path filename = ZAssetManager::GetRoot().pathAbsolute;
	filename = filename / "assets.xalocdb";

	XA_CORE_ERROR("Saving assets database: {0}", filename.string());

	pugi::xml_document doc = SerializeAssets(assetRegistry);
	bool success = doc.save_file(filename.c_str());

	if (!success)
	{
		XA_CORE_ERROR("Could not save assets database!");
	}
	else XA_CORE_INFO("Assets database saved", (const char*)filename.c_str());
}

}