#include "xapch.h"

#include "Xaloc/Renderer/Texture.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Xaloc { namespace XalocImGui {

    bool ImageButton(Ref<Texture2D>& texture, ImVec2 size, ImVec4 color)
    {
        return ImGui::ImageButton((void*)texture->GetRendererID(), size, { 1, 0 }, { 0, 1 }, -1, color);
    }

    bool ImageTreeNode(const char* label, Ref<Texture2D>& texture, float iconPadding)
    {
        //ImGuiState& g = *GImGui;
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        ImU32 id = window->GetID(label);
        ImVec2 pos = window->DC.CursorPos;
        pos.x += 4;
        ImRect bb(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x - 8, pos.y + ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2));

        bool opened = ImGui::TreeNodeBehaviorIsOpen(id);
        bool hovered, held;
        if (ImGui::ButtonBehavior(bb, id, &hovered, &held, true))
            window->DC.StateStorage->SetInt(id, opened ? 0 : 1);

        uint32_t colorIndex;
        if (held || hovered) colorIndex = ImGuiCol_HeaderHovered;
        //else if (opened) colorIndex = ImGuiCol_Header;
        else colorIndex = ImGuiCol_WindowBg;

        if (hovered || held || opened)
            window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[colorIndex]));

        // Icon, text
        float button_sz = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
        window->DrawList->AddImage(
            (void*)texture->GetRendererID(),
            { pos.x + iconPadding, pos.y + iconPadding },
            { pos.x + button_sz - iconPadding, pos.y + button_sz - iconPadding },
            { 0, 1 }, { 1, 0 }
        );
        ImGui::RenderText(ImVec2(pos.x + button_sz + ImGui::GetStyle().ItemInnerSpacing.x, pos.y + ImGui::GetStyle().FramePadding.y), label);

        ImGui::ItemSize(bb, ImGui::GetStyle().FramePadding.y);
        ImGui::ItemAdd(bb, id);

        if (opened)
            ImGui::TreePush(label);
        return opened;
    }

} }