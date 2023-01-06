#pragma once

#include "Xaloc.h"


namespace Xaloc {

	class EditorGamePreview
	{
	public:
		EditorGamePreview(const std::string& name);
		~EditorGamePreview() = default;

		inline bool IsFocused() const { return m_Focused; }
		inline bool IsHovered() const { return m_Hovered; }

		void OnImGuiDraw(Ref<Framebuffer>& framebuffer);

	private:
		ImVec2 CalculateFinalImageSize(ImVec2 viewportSize, Ref<Framebuffer>& framebuffer);


	private:
		std::string m_Name;

		int m_FrameSize[2];

		bool m_Focused = false;
		bool m_Hovered = false;
	};


}