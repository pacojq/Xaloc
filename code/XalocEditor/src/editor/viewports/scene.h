#pragma once
#include "xaloc/scene/entity.h"
#include "xaloc/scene/scene.h"
#include "xaloc/renderer/cameras/editor.h"
#include "xaloc/renderer/pipeline/framebuffer.h"

namespace Xaloc {
	class ZEditorCamera;
}

using namespace Xaloc;


namespace Editor
{

class ZEditorSceneView
{
public:
	ZEditorSceneView(const std::string& name);
	~ZEditorSceneView() = default;

	inline bool IsFocused() const { return m_Focused; }
	inline bool IsHovered() const { return m_Hovered; }

	inline const glm::vec2& GetSize() const { return m_Size; }
	inline const glm::vec2& GetBounds(int index) const { return m_Bounds[index]; }

	/// <returns>Whether the viewport has changed size.</returns>
	bool OnImGuiDraw(
		const ZFramebuffer* pFramebuffer,
		const ZRef<ZEditorCamera>& camera,
		const ZEntity& selectedEntity);

	std::pair<float, float> GetMouseViewportSpace();

	void SetScene(const ZRef<ZScene>& scene) { m_Scene = scene; }

private:
	std::string m_Name;
	ZRef<ZScene> m_Scene;

	bool m_Focused = false;
	bool m_Hovered = false;

	glm::vec2 m_Size = { 0.0f, 0.0f };
	glm::vec2 m_Bounds[2];
};

}