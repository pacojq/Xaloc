#pragma once

#include "Xaloc.h"


namespace Xaloc {

	class EditorViewport
	{
	public:
		EditorViewport(const std::string& name);
		~EditorViewport() = default;

		inline bool IsFocused() const { return m_Focused; }
		inline bool IsHovered() const { return m_Hovered; }

		inline const glm::vec2& GetSize() const { return m_Size; }
		inline const glm::vec2& GetBounds(int index) const { return m_Bounds[index]; }


		void Begin();
		void End();
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="renderPass"></param>
		/// <returns>Whether the viewport has changed size.</returns>
		bool Render(Ref<RenderPass>& renderPass);

		std::pair<float, float> GetMouseViewportSpace();
		
	private:
		std::string m_Name;
		
		bool m_Focused = false;
		bool m_Hovered = false;
		
		glm::vec2 m_Size = { 0.0f, 0.0f };
		glm::vec2 m_Bounds[2];
	};

	
}