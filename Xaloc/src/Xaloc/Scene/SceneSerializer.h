#pragma once

#include "Entity.h"

#include <pugixml.hpp>

namespace Xaloc {

	class Scene;

	class SceneSerializer
	{
	public:
		static Ref<Scene> Deserialize(pugi::xml_document& doc);
		static pugi::xml_document Serialize(const Ref<Scene>& scene);

	private:
		static void DeserializeEntity(pugi::xml_node& entityNode, const Ref<Scene>& scene);

		static void SerializeEntity(pugi::xml_document& doc, pugi::xml_node& entitiesRoot, 
			Entity entity, UUID id, const Ref<Scene>& scene);
	};
	
}