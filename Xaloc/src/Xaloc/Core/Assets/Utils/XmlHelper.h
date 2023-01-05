#pragma once

#include "Xaloc/Core/Assets/AssetMetadata.h"
#include "Xaloc/Scene/Components.h"

#include <pugixml.hpp>
#include <vector>

namespace Xaloc {

	class XmlHelper
	{
	public:
		static bool OpenXmlAsset(const AssetMetadata& metadata, pugi::xml_document& doc);

		static bool TryDeserializeCompTransform(pugi::xml_node& parentNode, TransformComponent& outTransform);
		static bool TryDeserializeCompSpriteRenderer(pugi::xml_node& parentNode, SpriteRendererComponent& outDrawing);

	public:
		static std::vector<float> ParseVector(const std::string& str);
		static uint64_t ParseUint64(const std::string& str);
	};

}