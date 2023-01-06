#pragma once

#include "Xaloc/Renderer/Texture.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Xaloc { namespace XalocImGui {

    bool ImageButton(Ref<Texture2D>& texture, ImVec2 size, ImVec4 color);

    bool ImageTreeNode(const char* id, Ref<Texture2D>& texture, float iconPadding = 0);

} }