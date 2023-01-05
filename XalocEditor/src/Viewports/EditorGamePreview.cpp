#include "xapch.h"
#include "EditorGamePreview.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


namespace Xaloc {

	EditorGamePreview::EditorGamePreview(const std::string& name)
		: m_Name(name)
	{

	}



	void EditorGamePreview::OnImGuiDraw(Ref<Framebuffer>& framebuffer)
	{
		if (m_FrameSize[0] <= 0 || m_FrameSize[1] <= 0)
		{
			m_FrameSize[0] = framebuffer->GetSpecification().Width;
			m_FrameSize[1] = framebuffer->GetSpecification().Height;
		}


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{ 500, 500 });
		ImGui::Begin(m_Name.c_str());

		m_Focused = ImGui::IsWindowFocused();
		m_Hovered = ImGui::IsWindowHovered();

		// ====== Header =============================================
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);
			ImGui::BeginChild("##header", ImVec2(ImGui::GetWindowWidth(), ImGui::GetTextLineHeight() + 10), true);
			{
				ImVec2 cursor = ImGui::GetCursorPos();
				cursor.y += 2;
				cursor.x += 2;
				ImGui::SetCursorPos(cursor);

				float h = ImGui::GetContentRegionAvail().y - 2;

				ImGui::BeginChild("##header_vp_size", ImVec2(340, h));
				ImGui::DragInt2("Viewport Size", m_FrameSize, 1, 1);
				ImGui::EndChild();

				ImGui::SameLine();

				float avail = ImGui::GetContentRegionAvail().x;
				if (avail > 220)
				{
					ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x - 220, h));
					ImGui::SameLine();

					if (ImGui::Button("Reset", ImVec2(90, h)))
					{
						m_FrameSize[0] = framebuffer->GetSpecification().Width;
						m_FrameSize[1] = framebuffer->GetSpecification().Height;
					}
					ImGui::SameLine();
					if (ImGui::Button("Apply", ImVec2(90, h)))
					{
						framebuffer->Resize((uint32_t)m_FrameSize[0], (uint32_t)m_FrameSize[1]);
					}
					ImGui::SameLine();
					ImGui::Dummy(ImVec2(20, h));
				}
			}
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}

		// ====== Preview ============================================
		{
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			ImVec2 finalSize = CalculateFinalImageSize(viewportSize, framebuffer);

			ImVec2 cursor = ImGui::GetCursorPos();
			cursor.y += (viewportSize.y - finalSize.y) / 2.0f;
			cursor.x += (viewportSize.x - finalSize.x) / 2.0f;
			ImGui::SetCursorPos(cursor);

			uint32_t texID = framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)texID, finalSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}




	ImVec2 EditorGamePreview::CalculateFinalImageSize(ImVec2 viewportSize, Ref<Framebuffer>& framebuffer)
	{
		glm::vec2 framebufferSize = glm::vec2(framebuffer->GetSpecification().Width, framebuffer->GetSpecification().Height);

		float ar_viewport = viewportSize.x / viewportSize.y;
		float ar_frame = framebufferSize.x / framebufferSize.y;

		ImVec2 finalSize = viewportSize;
		if (ar_viewport > ar_frame) // Wider, height constrains
		{
			finalSize.x = finalSize.y * (framebufferSize.x / framebufferSize.y);
		}
		else // Taller, width constrains
		{
			finalSize.y = viewportSize.x * (framebufferSize.y / framebufferSize.x);
		}

		return finalSize;
	}

}