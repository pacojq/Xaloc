#include "xapch.h"
#include "tooling/imgui/core.h"
#include "xaloc/debugging/assert.h"
#include "xaloc/renderer/resource/Texture.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


namespace Xaloc
{

bool ZImGui::ImageButton(const char* sID, const ZTexture2D* pTexture, ImVec2 size, ImVec4 color)
{
	return ImGui::ImageButton(sID, (void*)pTexture->GetRendererID(), size, { 1, 0 }, { 0, 1 }, ImVec4(0,0,0,0), color);
}

bool ZImGui::ImageTreeNode(
	const char* label,
	const ZTexture2D* pTexture,
	float iconPadding)
{
	//ImGuiState& g = *GImGui;
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	ImU32 id = window->GetID(label);
	ImVec2 pos = window->DC.CursorPos;
	pos.x += 4;
	ImRect bb(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x - 8, pos.y + ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2));

	int bOpened = window->DC.StateStorage->GetInt(id, 0) == 1;
	bool hovered, held;
	if (ImGui::ButtonBehavior(bb, id, &hovered, &held, true))
		window->DC.StateStorage->SetInt(id, bOpened ? 0 : 1);

	const uint32_t colorIndex = (held || hovered) ? ImGuiCol_HeaderHovered : ImGuiCol_WindowBg;
	if (hovered || held || bOpened)
	{
		window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[colorIndex]));
	}

	// Icon, text
	float button_sz = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
	window->DrawList->AddImage(
		(void*)pTexture->GetRendererID(),
		{ pos.x + iconPadding, pos.y + iconPadding },
		{ pos.x + button_sz - iconPadding, pos.y + button_sz - iconPadding },
		{ 0, 1 },
		{ 1, 0 });
	ImGui::RenderText(
		ImVec2(pos.x + button_sz + ImGui::GetStyle().ItemInnerSpacing.x, pos.y + ImGui::GetStyle().FramePadding.y), label);

	ImGui::ItemSize(bb, ImGui::GetStyle().FramePadding.y);
	ImGui::ItemAdd(bb, id);

	if (bOpened)
	{
		ImGui::TreePush(label);
	}
	return bOpened;
}

ZImGuiEnabledScope::ZImGuiEnabledScope(bool bEnabled)
	: m_bEnabled(bEnabled)
{
	if (!m_bEnabled)
	{
		ImGui::BeginDisabled();
	}
}

ZImGuiEnabledScope::~ZImGuiEnabledScope()
{
	if (!m_bEnabled)
	{
		XA_ASSERT(GImGui->DisabledStackSize > 0);
		ImGui::EndDisabled();
	}
}

}