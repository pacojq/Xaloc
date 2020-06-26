#include "xapch.h"
#include "SceneSerializer.h"

#include "Scene.h"

namespace Xaloc {

	Ref<Scene> SceneSerializer::Deserialize(pugi::xml_document& doc, pugi::xml_parse_result& result)
	{
		Ref<Scene> scene = CreateRef<Scene>("Sandbox Scene");

		return scene;
	}

}