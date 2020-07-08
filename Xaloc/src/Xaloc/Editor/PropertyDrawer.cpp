#include "xapch.h"
#include "PropertyDrawer.h"

#include <imgui.h>
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
		itoa(s_Counter++, s_IDBuffer + 2, 16);

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
		itoa(s_Counter++, s_IDBuffer + 2, 16);

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
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragInt(s_IDBuffer, &value))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}




	bool PropertyDrawer::Float(const char* label, float& value, float delta)
	{
		return Float(label, value, delta, FLOAT_MIN, FLOAT_MAX);
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
		itoa(s_Counter++, s_IDBuffer + 2, 16);
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

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat2(s_IDBuffer, glm::value_ptr(value), delta, min, max))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}



	bool PropertyDrawer::Vec3(const char* label, glm::vec3& value, float delta)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat3(s_IDBuffer, glm::value_ptr(value), delta))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool PropertyDrawer::Vec4(const char* label, glm::vec4& value, float delta)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat4(s_IDBuffer, glm::value_ptr(value), delta))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}
	
}