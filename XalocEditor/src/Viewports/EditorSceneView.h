#pragma once

#include "Xaloc.h"

#include "Xaloc/Renderer/Cameras/EditorCamera.h"


namespace Xaloc {

	class EditorSceneView
	{
	public:
		EditorSceneView(const std::string& name);
		~EditorSceneView() = default;

		inline bool IsFocused() const { return m_Focused; }
		inline bool IsHovered() const { return m_Hovered; }

		inline const glm::vec2& GetSize() const { return m_Size; }
		inline const glm::vec2& GetBounds(int index) const { return m_Bounds[index]; }

		/// <returns>Whether the viewport has changed size.</returns>
		bool OnImGuiDraw(const Ref<Framebuffer>& framebuffer, const Ref<EditorCamera>& camera, const Entity& selectedEntity);

		std::pair<float, float> GetMouseViewportSpace();

		void SetScene(const Ref<Scene>& scene) { m_Scene = scene; }

		
	private:
		std::string m_Name;
		Ref<Scene> m_Scene;
		
		bool m_Focused = false;
		bool m_Hovered = false;
		
		glm::vec2 m_Size = { 0.0f, 0.0f };
		glm::vec2 m_Bounds[2];
	};

	
}