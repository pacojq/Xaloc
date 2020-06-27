#pragma once

#include "Entity.h"

#include <pugixml.hpp>

namespace Xaloc {

	class Scene;

	class SceneSerializer
	{
	public:
		static Ref<Scene> Deserialize(pugi::xml_document& doc, pugi::xml_parse_result& result);
		static pugi::xml_document Serialize(const Ref<Scene>& scene);

	private:
		static void SerializeEntity(pugi::xml_document& doc, pugi::xml_node& entitiesRoot, 
			Entity entity, uint32_t id, const Ref<Scene>& scene);
	};
	
}