#pragma once
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui/imgui.h>

namespace Xaloc {
	class ZTexture2D;
}


namespace Xaloc
{

class ZImGui
{
public:
	static bool ImageButton(const char* sID, const ZTexture2D* pTexture, ImVec2 size, ImVec4 color);

	static bool ImageTreeNode(const char* sLabel, const ZTexture2D* pTexture, float iconPadding = 0);
};

class ZImGuiEnabledScope
{
public:
	ZImGuiEnabledScope(bool bEnabled);
	~ZImGuiEnabledScope();

private:
	bool m_bEnabled;
};

}