#include "xapch.h"
#include "AssetSerializers.h"

#include "AssetManager.h"
#include "Utils/XmlHelper.h"

#include "Xaloc/Renderer/Texture.h"
#include "Xaloc/Renderer/SubTexture2D.h"


#include <pugixml.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>


namespace Xaloc {

	bool TextureSerializer::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const
	{
		asset = Texture2D::Create(AssetManager::GetFileSystemPathString(metadata));
		asset->AssetHandle = metadata.Handle;

		/* TODO: error texture?
		Ref<Texture2D> whiteTexture = Texture2D::Create(1, 1);
                uint32_t whiteTextureData = 0xffffffff;
                whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		*/


		bool result = true;// asset.As<Texture2D>()->Loaded();

		if (!result)
			asset->SetAssetFlag(AssetFlag::Invalid, true);

		return result;
	}

	bool TextureSerializer::GetInvalidAsset(Ref<Asset>& asset)
	{
		if (m_InvalidAsset.get() == nullptr)
		{
			m_InvalidAsset = Texture2D::Create(1, 1);
			//uint32_t whiteTextureData = 0xffffffff;
			//s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		}
		asset = m_InvalidAsset;
		return true;
	}





	bool SpriteSerializer::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const
	{
		pugi::xml_document doc;
		XmlHelper::OpenXmlAsset(metadata, doc);

		pugi::xml_node root = doc.child("sprite");

		pugi::xml_node texNode = root.child("Texture2D");
		std::string strAssetID = texNode.attribute("assetID").value();
		uint64_t assetID = (UUID)XmlHelper::ParseUint64(strAssetID);
		Ref<Texture2D> tex = AssetManager::GetAsset<Texture2D>(assetID);

		pugi::xml_node cliprectNode = root.child("cliprect");
		std::string strTexcoords = cliprectNode.attribute("texcoords").value();
		auto texcoords = XmlHelper::ParseVector(strTexcoords);
		uint32_t width = cliprectNode.attribute("width").as_uint();
		uint32_t height = cliprectNode.attribute("height").as_uint();


		asset = SubTexture2D::CreateFromAbsCoords(tex,
			{ texcoords[0] * tex->GetWidth(), texcoords[1] * tex->GetHeight() }, { width, height });
		asset->AssetHandle = metadata.Handle;

		bool result = true;// asset.As<Texture2D>()->Loaded();
		if (!result) asset->SetAssetFlag(AssetFlag::Invalid, true);

		return result;
	}






	void PrefabSerializer::Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset) const
	{
		// TODO
	}

	bool PrefabSerializer::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const
	{
		pugi::xml_document doc;
		XmlHelper::OpenXmlAsset(metadata, doc);

		//Ref<PrefabAsset> prefabAsset = CreateRef<PrefabAsset>();
		//asset = prefabAsset;
		//asset->AssetHandle = metadata.Handle;

		pugi::xml_node root = doc.child("entity");

		std::string name = root.child("name").child_value();
		//prefabAsset->Name = name;


		// TODO: deserialize components


		bool result = true;// TODO
		if (!result)
			asset->SetAssetFlag(AssetFlag::Invalid, true);
		return result;
	}

	bool PrefabSerializer::GetInvalidAsset(Ref<Asset>& asset)
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