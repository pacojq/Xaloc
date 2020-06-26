#pragma once

#include <pugixml.hpp>

namespace Xaloc {

	class Scene;

	class SceneSerializer
	{
	public:
		static Ref<Scene> Deserialize(pugi::xml_document& doc, pugi::xml_parse_result& result);
	};
	
}