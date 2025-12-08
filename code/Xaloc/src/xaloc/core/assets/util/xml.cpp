#include "xapch.h"
#include "xaloc/core/assets/util/parsing.h"
#include "xaloc/core/assets/util/xml.h"
#include "xaloc/core/assets/manager.h"
#include "xaloc/debugging/assert.h"
#include <fstream>
#include <iomanip>
#include <iostream>


namespace Xaloc
{

static std::string Uint64ToString(uint64_t n)
{
	char buff[16 + 1];
	snprintf(buff, sizeof(buff), "%llx", n);
	std::string result = buff;
	return result;
}

bool XmlOpenAsset(
	pugi::xml_document& rDocument,
	const SAssetMetadata& metadata)
{
	std::ifstream stream(ZAssetManager::GetFileSystemPathString(metadata));
	pugi::xml_parse_result docResult = rDocument.load(stream);

	XA_CORE_TRACE("Asset read file result: {}", docResult.description());
	if (docResult.status != pugi::xml_parse_status::status_ok)
	{
		if (docResult.status == pugi::xml_parse_status::status_file_not_found)
		{
			XA_CORE_ASSERT(false, "Could not load XML. File not found.");
		}
		else
		{
			XA_CORE_ASSERT(false, "Could not load XML.");
		}
		return false;
	}

	return true;
}

bool XmlDeserializeTransform(
	CmpTransform& rTransform,
	const pugi::xml_node& nodeParent)
{
	pugi::xml_node nodeTransform = nodeParent.child("Transform");
	if (nodeTransform)
	{
		std::string strPosition = nodeTransform.attribute("position").value();
		auto vecPosition = ParseVector(strPosition);
		glm::vec3 position = { vecPosition[0], vecPosition[1], vecPosition[2] };

		std::string strRotation = nodeTransform.attribute("rotation").value();
		auto vecRotation = ParseVector(strRotation);
		glm::vec3 rotation = { vecRotation[0], vecRotation[1], vecRotation[2] };

		std::string strScale = nodeTransform.attribute("scale").value();
		auto vecScale = ParseVector(strScale);
		glm::vec3 scale = { vecScale[0], vecScale[1], vecScale[2] };

		rTransform = CmpTransform();
		rTransform.Translation = position;
		rTransform.Rotation = rotation;
		rTransform.Scale = scale;

		//XA_CORE_TRACE("        Transform: position = {0}	rotation = {1}	scale = {2}",
		//	strPosition, strRotation, strScale);
		
		return true;
	}
	return false;
}

bool XmlDeserializeSpriteRenderer(
	CmpSpriteRenderer& rSpriteRenderer,
	const pugi::xml_node& nodeParent)
{
	pugi::xml_node nodeSpriteRenderer = nodeParent.child("SpriteRenderer");
	if (nodeSpriteRenderer)
	{
		//std::string strTexcoords = rendererNode.attribute("texcoords").value();
		//auto texcoords = ParseVector(strTexcoords);

		//uint32_t width = rendererNode.attribute("width").as_uint();
		//uint32_t height = rendererNode.attribute("height").as_uint();

		std::string strSpriteAssetID = nodeSpriteRenderer.attribute("sprite").value();
		uint64_t spriteAssetID = (ZGuid)ParseUint64(strSpriteAssetID);

		if (ZAssetManager::ReadMetadata(spriteAssetID).IsValid())
		{
			rSpriteRenderer = CmpSpriteRenderer();
			rSpriteRenderer.Sprite = spriteAssetID;
		}
		else
		{
			XA_CORE_ERROR("Sprite renderer could not load sprite with asset ID '{0}', spriteAssetID");

			// TODO load with error texture/material

			rSpriteRenderer = CmpSpriteRenderer();
			rSpriteRenderer.Sprite = 0;
		}

		//XA_CORE_TRACE("        SpriteRenderer: assetID = {0}", spriteAssetID);

		return true;
	}
	return false;
}

}