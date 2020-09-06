#include "xapch.h"
#include "EditorViewport.h"

namespace Xaloc {

	EditorViewport::EditorViewport(const std::string& name)
		: m_Name(name)
	{
		
	}


	void EditorViewport::Begin()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(m_Name.c_str());

		m_Focused = ImGui::IsWindowFocused();
		m_Hovered = ImGui::IsWindowHovered();
	}

	
	bool EditorViewport::Render(Ref<RenderPass>& renderPass)
	{
		auto viewportOffset = ImGui::GetCursorPos();
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		bool resized = false;
		
		if (m_Size != *((glm::vec2*) & viewportSize))
		{
			renderPass->GetSpecification().TargetFramebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			//m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_Size = { viewportSize.x, viewportSize.y };

			resized = true;
		}

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };

		m_Bounds[0] = { minBound.x, minBound.y };
		m_Bounds[1] = { maxBound.x, maxBound.y };

		// TODO m_AllowViewportCameraEvents = ImGui::IsMouseHoveringRect(minBound, maxBound);

		uint32_t texID = renderPass->GetSpecification().TargetFramebuffer->GetColorAttachmentRendererID();
		//uint32_t texID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)texID, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		

		return resized;
	}

	void EditorViewport::End()
	{
		ImGui::End();
		ImGui::PopStyleVar();
	}

	


	std::pair<float, float> EditorViewport::GetMouseViewportSpace()
	{
		auto [mx, my] = ImGui::GetMousePos(); // Input::GetMousePosition();
		mx -= m_Bounds[0].x;
		my -= m_Bounds[0].y;
		auto viewportWidth = m_Bounds[1].x - m_Bounds[0].x;
		auto viewportHeight = m_Bounds[1].y - m_Bounds[0].y;

		return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
	}
}