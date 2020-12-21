#include "PropertyDrawer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <limits>

namespace Xaloc {

	#define FLOAT_MIN std::numeric_limits<float>::min()
	#define FLOAT_MAX std::numeric_limits<float>::max()
	
	
	static int s_UIContextID = 0;
	static uint32_t s_Counter = 0;
	static char s_IDBuffer[16];

	static void PushID()
	{
		ImGui::PushID(s_UIContextID++);
		s_Counter = 0;
	}

	static void PopID()
	{
		ImGui::PopID();
		s_UIContextID--;
	}


	void PropertyDrawer::BeginPropertyGrid()
	{
		PushID();
		ImGui::Columns(2);
	}

	void PropertyDrawer::EndPropertyGrid()
	{
		ImGui::Columns(1);
		PopID();
	}





	

	bool PropertyDrawer::String(const char* label, std::string& value, bool error)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		char buffer[256];
		strcpy(buffer, value.c_str());

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);

		if (error) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		
		if (ImGui::InputText(s_IDBuffer, buffer, 256))
		{
			value = buffer;
			modified = true;
		}

		if (error) ImGui::PopStyleColor();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool PropertyDrawer::String(const char* label, const char* value, bool error)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);

		if (error) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		
		if (ImGui::InputText(s_IDBuffer, (char*)value, 256, ImGuiInputTextFlags_ReadOnly))
			modified = true;

		if (error) ImGui::PopStyleColor();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}




	

	bool PropertyDrawer::Int(const char* label, int& value)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragInt(s_IDBuffer, &value))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}




	bool PropertyDrawer::Float(const char* label, float& value, float delta)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat(s_IDBuffer, &value, delta))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool PropertyDrawer::Float(const char* label, float& value, float delta, float min)
	{
		return Float(label, value, delta, min, FLOAT_MAX);
	}


	bool PropertyDrawer::Float(const char* label, float& value, float delta, float min, float max)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat(s_IDBuffer, &value, delta, min, max))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}






	bool PropertyDrawer::Vec2(const char* label, glm::vec2& value, float delta)
	{
		return Vec2(label, value, delta, FLOAT_MIN, FLOAT_MAX);
	}

	bool PropertyDrawer::Vec2(const char* label, glm::vec2& value, float delta, float min)
	{
		return Vec2(label, value, delta, min, FLOAT_MAX);
	}


	bool PropertyDrawer::Vec2(const char* label, glm::vec2& value, float delta, float min, float max)
	{
		bool modified = false;

		ImGui::PushID(label);
		ImGui::Text(label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		modified |= VecCoord(0, value.x, delta);
		modified |= VecCoord(1, value.y, delta);

		ImGui::NextColumn();
		ImGui::PopStyleVar();
		ImGui::PopID();

		return modified;
	}




		
	bool PropertyDrawer::Vec3(const char* label, glm::vec3& value, float delta)
	{
		bool modified = false;

		ImGui::PushID(label);
		ImGui::Text(label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		modified |= VecCoord(0, value.x, delta);
		modified |= VecCoord(1, value.y, delta);
		modified |= VecCoord(2, value.z, delta);

		ImGui::NextColumn();
		ImGui::PopStyleVar();
		ImGui::PopID();
		
		return modified;
	}




	bool PropertyDrawer::Vec4(const char* label, glm::vec4& value, float delta)
	{
		bool modified = false;

		ImGui::PushID(label);
		ImGui::Text(label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		modified |= VecCoord(0, value.x, delta);
		modified |= VecCoord(1, value.y, delta);
		modified |= VecCoord(2, value.z, delta);
		modified |= VecCoord(3, value.w, delta);

		ImGui::NextColumn();
		ImGui::PopStyleVar();
		ImGui::PopID();

		return modified;
	}





	bool PropertyDrawer::ComboBox(const char* label, std::vector<std::string> values, const std::string currentValue, int* outIndex)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::BeginCombo(s_IDBuffer, currentValue.c_str()))
		{
			int index = -1;
			for (std::string str : values)
			{
				index++;
				bool isSelected = str.compare(currentValue) == 0;
				
				if (ImGui::Selectable(str.c_str(), isSelected))
				{
					outIndex = &index;
					modified = true;
				}
				if (isSelected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		
		return modified;
	}













	bool PropertyDrawer::VecCoord(int coord, float& value, float delta)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		bool modified = false;

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, s_Coords[coord].Color);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, s_Coords[coord].ColorHovered);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, s_Coords[coord].ColorActive);
		ImGui::PushFont(boldFont);
		if (ImGui::Button(s_Coords[coord].Label, buttonSize))
		{
			value = 0.0f;// TODO reset value resetValue;
			modified = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat(s_Coords[coord].Id, &value, delta, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		return modified;
	}
	
}