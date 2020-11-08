#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Scene/Scene.h"
#include "Xaloc/Scene/Entity.h"

#include <imgui/imgui.h>

namespace Xaloc {

	/// <summary>
	/// Used to draw vector properties.
	/// </summary>
	struct PropertyDrawerCoord
	{
		ImVec4 Color;
		ImVec4 ColorHovered;
		ImVec4 ColorActive;

		char* Label;
		char* Id;
	};
	
	
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

		
	private:
		
		/// <summary>
		/// Draws a vector coordinate.
		/// </summary>
		/// <param name="coord"></param>
		/// <param name="value"></param>
		/// <param name="delta"></param>
		/// <returns></returns>
		static bool VecCoord(int coord, float& value, float delta);



		
	private:

		/// <summary>
		/// Coordinate drawing properties.
		/// </summary>
		inline static PropertyDrawerCoord s_Coords[] = {
			{
				ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f },
				ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f },
				ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f },
				"X",
				"##X"
			},
			{
				ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f },
				ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f },
				ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f },
				"Y",
				"##Y"
			},
			{
				ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f },
				ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f },
				ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f },
				"Z",
				"##Z"
			},
			{
				ImVec4{ 0.78f, 0.78f, 0.78f, 1.0f },
				ImVec4{ 0.93f, 0.93f, 0.93f, 1.0f },
				ImVec4{ 0.56f, 0.56f, 0.56f, 1.0f },
				"W",
				"##W"
			}
		};
	};

}