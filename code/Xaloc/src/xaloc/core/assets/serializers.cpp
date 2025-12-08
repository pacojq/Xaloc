#include "xapch.h"
#include "xaloc/core/assets/serializers.h"
#include "xaloc/core/assets/manager.h"
#include "xaloc/core/assets/metadata.h"
#include "xaloc/core/assets/util/parsing.h"
#include "xaloc/core/assets/util/xml.h"
#include "xaloc/renderer/resource/texture.h"
#include "xaloc/renderer/resource/subtexture.h"
#include <pugixml.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>


namespace Xaloc
{

bool ZAssetSerializerTexture::TryLoadData(
	ZRef<Asset>& rAsset,
	const SAssetMetadata& metadata) const
{
	const std::string sPath = ZAssetManager::GetFileSystemPathString(metadata);
	rAsset = ZTexture2D::Create(sPath.c_str());
	rAsset->AssetHandle = metadata.nHandle;

	/* TODO: error texture?
	ZRef<Texture2D> whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
	*/

	bool result = true;// asset.As<Texture2D>()->Loaded();

	if (!result)
	{
		rAsset->SetAssetFlag(AssetFlag::Invalid, true);
	}
	return result;
}

bool ZAssetSerializerTexture::GetInvalidAsset(
	ZRef<Asset>& rAsset)
{
	if (m_InvalidAsset.get() == nullptr)
	{
		m_InvalidAsset = ZTexture2D::Create(1, 1);
		//uint32_t whiteTextureData = 0xffffffff;
		//s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
	}
	rAsset = m_InvalidAsset;
	return true;
}

bool ZAssetSerializerSprite::TryLoadData(
	ZRef<Asset>& rAsset,
	const SAssetMetadata& metadata) const
{
	pugi::xml_document doc;
	XmlOpenAsset(doc, metadata);

	pugi::xml_node root = doc.child("sprite");

	pugi::xml_node texNode = root.child("Texture2D");
	std::string strAssetID = texNode.attribute("assetID").value();
	uint64_t assetID = (ZGuid)ParseUint64(strAssetID);
	ZRef<ZTexture2D> tex = ZAssetManager::GetAsset<ZTexture2D>(assetID);

	pugi::xml_node cliprectNode = root.child("cliprect");
	std::string strTexcoords = cliprectNode.attribute("texcoords").value();
	auto texcoords = ParseVector(strTexcoords);
	uint32_t width = cliprectNode.attribute("width").as_uint();
	uint32_t height = cliprectNode.attribute("height").as_uint();

	rAsset = ZSubTexture2D::CreateFromAbsCoords(
		tex,
		{ 
			texcoords[0] * tex->GetWidth(),
			texcoords[1] * tex->GetHeight()
		},
		{ 
			width,
			height
		});
	rAsset->AssetHandle = metadata.nHandle;

	bool result = true;// asset.As<Texture2D>()->Loaded();
	if (!result)
	{
		rAsset->SetAssetFlag(AssetFlag::Invalid, true);
	}
	return result;
}

void ZAssetSerializerPrefab::Serialize(
	const SAssetMetadata& metadata,
	const ZRef<Asset>& asset) const
{
	// TODO
}

bool ZAssetSerializerPrefab::TryLoadData(
	ZRef<Asset>& rAsset,
	const SAssetMetadata& metadata) const
{
	pugi::xml_document doc;
	XmlOpenAsset(doc, metadata);

	//ZRef<PrefabAsset> prefabAsset = CreateRef<PrefabAsset>();
	//asset = prefabAsset;
	//asset->AssetHandle = metadata.Handle;

	pugi::xml_node root = doc.child("entity");

	std::string name = root.child("name").child_value();
	//prefabAsset->Name = name;

	// TODO: deserialize components

	bool result = true;// TODO
	if (!result)
	{
		rAsset->SetAssetFlag(AssetFlag::Invalid, true);
	}
	return result;
}

bool ZAssetSerializerPrefab::GetInvalidAsset(
	ZRef<Asset>& rAsset)
{
	return false;
	/*
	if (m_InvalidAsset.get() == nullptr)
	{
		//m_InvalidAsset = CreateRef<PrefabAsset>();
	}
	asset = m_InvalidAsset;
	return true;
	*/
}

}