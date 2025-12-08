#pragma once
#include <string>

namespace Xaloc {
	class ZFramebuffer;
}

using namespace Xaloc;


namespace Editor
{

class ZEditorGamePreview
{
public:
	ZEditorGamePreview(const std::string& name);
	~ZEditorGamePreview() = default;

	inline bool IsFocused() const { return m_Focused; }
	inline bool IsHovered() const { return m_Hovered; }

	void OnImGuiDraw(ZFramebuffer* pFramebuffer);

private:
	std::string m_Name;

	int m_FrameSize[2];

	bool m_Focused = false;
	bool m_Hovered = false;
};

}