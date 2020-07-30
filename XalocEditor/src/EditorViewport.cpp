#include "xapch.h"
#include "EditorViewport.h"

namespace Xaloc {

	EditorViewport::EditorViewport(const std::string& name)
		: m_Name(name)
	{
		
	}

	
	bool EditorViewport::Render(Ref<RenderPass>& renderPass)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(m_Name.c_str());

		m_Focused = ImGui::IsWindowFocused();
		m_Hovered = ImGui::IsWindowHovered();

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
		ImGui::End();
		ImGui::PopStyleVar();

		return resized;
	}

	
}