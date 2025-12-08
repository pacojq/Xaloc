#pragma once
#include "xaloc/core/guid.h"
#include "xaloc/scene/entity.h"
#include <pugixml.hpp>

namespace Xaloc {
	class ZScene;
}


namespace Xaloc
{

class ZSceneSerializer
{
public:
	static ZRef<ZScene> Deserialize(pugi::xml_document& doc);
	static pugi::xml_document Serialize(const ZRef<ZScene>& scene);

private:
	static void DeserializeEntity(
		pugi::xml_node& entityNode,
		const ZRef<ZScene>& scene);

	static void SerializeEntity(
		pugi::xml_document& doc,
		pugi::xml_node& entitiesRoot, 
		ZEntity entity,
		ZGuid id,
		const ZRef<ZScene>& scene);
};

}