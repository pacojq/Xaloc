#include "xapch.h"
#include "tooling/propertydrawer.h"
#include "tooling/imgui/core.h"
#include "xaloc/core/assets/manager.h"
#include "xaloc/core/hashing.h"
#include "xaloc/scene/scene.h"
#include "xaloc/scene/entity.h"
#include "xaloc/math/math.h"
#include <limits>

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"


namespace Xaloc
{

static uint32_t s_Counter = 0;
static char s_IDBuffer[16];

/// <summary>
/// Coordinate drawing properties.
/// </summary>
static PropertyDrawerCoord s_aCoords[] ={
	{
		ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f },
		ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f },
		ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f },
		"x",
		"##x",
		"##vx"
	},
	{
		ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f },
		ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f },
		ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f },
		"y",
		"##y",
		"##vy"
	},
	{
		ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f },
		ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f },
		ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f },
		"z",
		"##z",
		"##vz"
	},
	{
		ImVec4{ 0.78f, 0.78f, 0.78f, 1.0f },
		ImVec4{ 0.93f, 0.93f, 0.93f, 1.0f },
		ImVec4{ 0.56f, 0.56f, 0.56f, 1.0f },
		"w",
		"##w",
		"##vw"
	}
};

/// <summary>
/// Draws a vector coordinate.
/// </summary>
/// <param name="coord"></param>
/// <param name="value"></param>
/// <param name="delta"></param>
/// <returns></returns>
static bool VecCoord(
	int coord,
	float& rValue,
	float delta,
	float32 fAvailableWidth)
{
	const float32 fWidthCollapsedThreshold = 74.0f;
	const bool bCollapsed = fAvailableWidth < fWidthCollapsedThreshold;

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	bool bModified = false;

	const float32 fLineHeight = GImGui->Font->LegacySize
		+ GImGui->Style.FramePadding.y * 2.0f;
	const float32 fButtonWidth = bCollapsed ? 6.0f : fLineHeight + 3.0f;
	const ImVec2 buttonSize = { fButtonWidth, fLineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button,        s_aCoords[coord].color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, s_aCoords[coord].colorHovered);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,  s_aCoords[coord].colorActive);
	ImGui::PushFont(boldFont);

	const char* sLabel = bCollapsed ? s_aCoords[coord].sLabelId : s_aCoords[coord].sLabel;

	ImGui::SetNextItemWidth(fButtonWidth);
	if (ImGui::Button(sLabel, buttonSize))
	{
		rValue = 0.0f;// TODO reset value to a parametrized default, instead of zero
		bModified = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::SetNextItemWidth(fAvailableWidth - fButtonWidth);
	bModified |= ImGui::DragFloat(
		s_aCoords[coord].sValueId,
		&rValue,
		delta,
		0.0f,
		0.0f,
		"%.2f");
	ImGui::SameLine();

	return bModified;
}

ZPropertyDrawer::ZPropertyDrawer(const char* sName)
	: m_bOpen(false)
{
	m_nId = HashFromString(sName);
}

ZPropertyDrawer::~ZPropertyDrawer()
{
	XA_ASSERT(!m_bOpen, "Property Drawer was destroyed without calling `EndPropertyGrid()`!");
}

void ZPropertyDrawer::Begin()
{
	m_bOpen = true;
	ImGui::PushID(int32(m_nId));
	s_Counter = 0;
	ImGui::Columns(2);
}

void ZPropertyDrawer::End()
{
	ImGui::Columns(1);
	ImGui::PopID();
	m_bOpen = false;
}

void ZPropertyDrawer::OpenDisabledScope(bool bDdisabled)
{
	ImGui::BeginDisabled(bDdisabled);
}

void ZPropertyDrawer::CloseDisabledScope()
{
	XA_ASSERT(GImGui->DisabledStackSize > 0);
	ImGui::EndDisabled();
}

void ZPropertyDrawer::Guid(const char* sLabel, ZGuid nGuid, bool bError)
{
	ImGui::Text(sLabel);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	if (bError)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
	}

	ImGui::Text("%llx", nGuid);

	if (bError)
	{
		ImGui::PopStyleColor();
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();
}

bool ZPropertyDrawer::Asset(
	const char* sLabel,
	const EAssetType eType,
	ZAssetHandle& rValue,
	void* icon)
{
	bool bModified = false;

	float y = ImGui::GetCursorPosY();
	y += (ImGui::GetTextLineHeightWithSpacing() - ImGui::GetTextLineHeight()) / 2.0f;
	ImGui::SetCursorPosY(y);

	ImGui::Text(sLabel);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	if (icon != nullptr)
	{
		float height = ImGui::GetTextLineHeightWithSpacing();
		ImGui::Image(icon, ImVec2(height, height), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::SameLine();
	}

	char buffer[256];

	if (rValue == 0)
	{
		strcpy(buffer, "None");
	}
	else
	{
		std::string name = ZAssetManager::ReadMetadata(rValue).pathResource.filename().string();
		strcpy(buffer, (char*)name.c_str());
	}
	ImGui::BeginDisabled();
	ImGui::InputText("##asset", buffer, 256, ImGuiInputTextFlags_ReadOnly);
	ImGui::EndDisabled();

	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
	{
		XA_CORE_WARN("Clicked!!");
		// TODO: open editor, maybe editor
	}

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("selectable");
		if (payload)
		{
			const std::string absPath = (char*)payload->Data;
			XA_CORE_WARN("{}", absPath);

			if (eType == ZAssetManager::GetAssetTypeFromPath(absPath))
			{
				rValue = ZAssetManager::GetAssetHandleFromFilePath(absPath);
				bModified = true;
			}
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return bModified;
}

bool ZPropertyDrawer::String(
	const char* sLabel,
	std::string& rValue,
	bool error)
{
	bool bModified = false;

	ImGui::Text(sLabel);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	char buffer[256];
	strcpy(buffer, rValue.c_str());

	s_IDBuffer[0] = '#';
	s_IDBuffer[1] = '#';
	memset(s_IDBuffer + 2, 0, 14);
	_itoa(s_Counter++, s_IDBuffer + 2, 16);

	if (error) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		
	if (ImGui::InputText(s_IDBuffer, buffer, 256))
	{
		rValue = buffer;
		bModified = true;
	}

	if (error)
	{
		ImGui::PopStyleColor();
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return bModified;
}
/*
bool ZPropertyDrawer::String(
	const char* sLabel,
	const char* sValue,
	bool error)
{
	bool bModified = false;

	ImGui::Text(sLabel);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	s_IDBuffer[0] = '#';
	s_IDBuffer[1] = '#';
	memset(s_IDBuffer + 2, 0, 14);
	_itoa(s_Counter++, s_IDBuffer + 2, 16);

	if (error) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		
	if (ImGui::InputText(s_IDBuffer, (char*)sValue, 256, ImGuiInputTextFlags_ReadOnly))
		bModified = true;

	if (error) ImGui::PopStyleColor();

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return bModified;
}
*/

bool ZPropertyDrawer::Int(const char* sLabel, int& rValue, int delta)
{
	bool bModified = false;

	ImGui::Text(sLabel);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	std::string id = fmt::format("##{0}", sLabel);
	if (ImGui::DragInt(id.c_str(), &rValue, delta))
	{
		bModified = true;
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return bModified;
}

bool ZPropertyDrawer::Int(
	const char* sLabel,
	int& rValue,
	int delta,
	int min)
{
	return Int(sLabel, rValue, delta, min, INT_MAX);
}


bool ZPropertyDrawer::Int(
	const char* sLabel,
	int& rValue, int delta, int min, int max)
{
	bool bModified = false;

	ImGui::Text(sLabel);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	std::string id = fmt::format("##{0}", sLabel);
	if (ImGui::DragInt(id.c_str(), &rValue, delta, min, max))
	{
		bModified = true;
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return bModified;
}

bool ZPropertyDrawer::Float(
	const char* sLabel,
	float& rValue,
	float delta)
{
	bool bModified = false;

	ImGui::Text(sLabel);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	std::string id = fmt::format("##{0}", sLabel);
	if (ImGui::DragFloat(id.c_str(), &rValue, delta))
	{
		bModified = true;
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return bModified;
}

bool ZPropertyDrawer::Float(
	const char* sLabel,
	float& rValue,
	float delta,
	float min)
{
	return Float(sLabel, rValue, delta, min, C_FLOAT_MAX);
}

bool ZPropertyDrawer::Float(
	const char* sLabel,
	float& rValue,
	float delta,
	float min,
	float max)
{
	bool bModified = false;

	ImGui::Text(sLabel);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	std::string id = fmt::format("##{0}", sLabel);
	if (ImGui::DragFloat(id.c_str(), &rValue, delta, min, max))
	{
		bModified = true;
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return bModified;
}

bool ZPropertyDrawer::Vec2(
	const char* sLabel,
	glm::vec2& rValue,
	float delta)
{
	return Vec2(sLabel, rValue, delta, C_FLOAT_MIN, C_FLOAT_MAX);
}

bool ZPropertyDrawer::Vec2(
	const char* sLabel,
	glm::vec2& rValue,
	float delta,
	float min)
{
	return Vec2(sLabel, rValue, delta, min, C_FLOAT_MAX);
}

bool ZPropertyDrawer::Vec2(
	const char* sLabel,
	glm::vec2& rValue,
	float delta,
	float min,
	float max)
{
	bool bModified = false;

	ImGui::PushID(sLabel);
	ImGui::Text(sLabel);
	ImGui::NextColumn();

	const float32 fWidth = ImGui::CalcItemWidth();
	const float32 fWidthItem = fWidth / 2.0f;
	//ImGui::PushMultiItemsWidths(1, fWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	bModified |= VecCoord(0, rValue.x, delta, fWidthItem);
	bModified |= VecCoord(1, rValue.y, delta, fWidthItem);

	ImGui::NextColumn();
	ImGui::PopStyleVar();
	ImGui::PopID();

	return bModified;
}

bool ZPropertyDrawer::Vec3(
	const char* sLabel,
	glm::vec3& rValue,
	float delta)
{
	bool bModified = false;

	ImGui::PushID(sLabel);
	ImGui::Text(sLabel);
	ImGui::NextColumn();

	const float32 fWidth = ImGui::CalcItemWidth();
	const float32 fWidthItem = fWidth / 3.0f;
	//ImGui::PushMultiItemsWidths(2, fWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	bModified |= VecCoord(0, rValue.x, delta, fWidthItem);
	bModified |= VecCoord(1, rValue.y, delta, fWidthItem);
	bModified |= VecCoord(2, rValue.z, delta, fWidthItem);

	ImGui::NextColumn();
	ImGui::PopStyleVar();
	ImGui::PopID();

	return bModified;
}

bool ZPropertyDrawer::Vec4(
	const char* sLabel,
	glm::vec4& rValue,
	float delta)
{
	bool bModified = false;

	ImGui::PushID(sLabel);
	ImGui::Text(sLabel);
	ImGui::NextColumn();

	const float32 fWidth = ImGui::CalcItemWidth();
	const float32 fWidthItem = fWidth / 4.0f;
	//ImGui::PushMultiItemsWidths(3, fWidth);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	bModified |= VecCoord(0, rValue.x, delta, fWidthItem);
	bModified |= VecCoord(1, rValue.y, delta, fWidthItem);
	bModified |= VecCoord(2, rValue.z, delta, fWidthItem);
	bModified |= VecCoord(3, rValue.w, delta, fWidthItem);

	ImGui::NextColumn();
	ImGui::PopStyleVar();
	ImGui::PopID();

	return bModified;
}

bool ZPropertyDrawer::ComboBox(
	int32& rSelectedIndex,
	const char* sLabel,
	const std::vector<std::string>& aValues,
	const std::string& sCurrentValue)
{
	bool bModified = false;

	ImGui::Text(sLabel);
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	s_IDBuffer[0] = '#';
	s_IDBuffer[1] = '#';
	memset(s_IDBuffer + 2, 0, 14);
	_itoa(s_Counter++, s_IDBuffer + 2, 16);
	if (ImGui::BeginCombo(s_IDBuffer, sCurrentValue.c_str()))
	{
		int index = -1;
		for (std::string str : aValues)
		{
			index++;
			bool isSelected = str.compare(sCurrentValue) == 0;

			if (ImGui::Selectable(str.c_str(), isSelected))
			{
				rSelectedIndex = index;
				bModified = true;
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return bModified;
}
	
}