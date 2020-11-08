#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Scene/Scene.h"
#include "Xaloc/Scene/Entity.h"

namespace Xaloc {
	
	class PropertyDrawer
	{
	public:
		static void BeginPropertyGrid();
		static void EndPropertyGrid();
		
		static bool String(const char* label, const char* value, bool error = false);
		static bool String(const char* label, std::string& value, bool error = false);

		static bool Int(const char* label, int& value);

		static bool Float(const char* label, float& value, float delta = 0.1f);
		static bool Float(const char* label, float& value, float delta, float min);
		static bool Float(const char* label, float& value, float delta, float min, float max);

		static bool Vec2(const char* label, glm::vec2& value, float delta = 0.1f);
		static bool Vec2(const char* label, glm::vec2& value, float delta, float min);
		static bool Vec2(const char* label, glm::vec2& value, float delta, float min, float max);

		static bool Vec3(const char* label, glm::vec3& value, float delta = 0.1f);
		static bool Vec4(const char* label, glm::vec4& value, float delta = 0.1f);

		static bool ComboBox(const char* label, std::vector<std::string> values, const std::string currentValue, int* outIndex);
	};

}