#pragma once
#include "xaloc/core/assets/metadata.h"
#include "xaloc/scene/components.h"
#include <pugixml.hpp>
#include <vector>


namespace Xaloc
{

bool XmlOpenAsset(
	pugi::xml_document& rDocument,
	const SAssetMetadata& metadata);

bool XmlDeserializeTransform(
	CmpTransform& rTransform,
	const pugi::xml_node& nodeParent);

bool XmlDeserializeSpriteRenderer(
	CmpSpriteRenderer& rSpriteRenderer,
	const pugi::xml_node& nodeParent);

}