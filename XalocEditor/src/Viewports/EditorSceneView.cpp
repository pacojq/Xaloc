#include "xapch.h"
#include "EditorSceneView.h"

#include "Xaloc/Scene/Components.h"

#include "Xaloc/ImGui/ImGuizmo.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


namespace Xaloc {

	static const float TOOLBAR_HEIGHT = 32.0f;

	EditorSceneView::EditorSceneView(const std::string& name)
		: m_Name(name)
	{
		
	}


	
	bool EditorSceneView::OnImGuiDraw(const Ref<Framebuffer>& framebuffer, const Ref<EditorCamera>& camera,
		const Entity& selectedEntity)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(m_Name.c_str());

		m_Focused = ImGui::IsWindowFocused();
		m_Hovered = ImGui::IsWindowHovered();

		auto viewportOffset = ImGui::GetCursorPos();
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();


		bool resized = false;
		if (m_Size != *((glm::vec2*)&viewportSize))
		{
			framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
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

		uint32_t texID = framebuffer->GetColorAttachmentRendererID();
		//uint32_t texID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)texID, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


		// ============================================= GUIZMO ============================================= //

		const bool isOrthographic = camera->GetProjectionType() == EditorCamera::ProjectionType::Orthographic;

		// Editor Camera
		const glm::mat4& cameraProjection = camera->GetProjection();
		glm::mat4 cameraView = camera->GetViewMatrix();

		float rw = (float)viewportSize.x;
		float rh = (float)viewportSize.y;
		ImGuizmo::SetOrthographic(isOrthographic);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(minBound.x, minBound.y, rw, rh);

		// Guizmo grid
		//float gizmoGridScale = 1.0f;// + glm::log(m_EditorCamera.GetDistance());
		//glm::mat4 gizmoGridTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
		//	* glm::rotate(glm::mat4(1.0f), 0.0f, { 0.0f, 0.0f, 1.0f })
		//	* glm::scale(glm::mat4(1.0f), { gizmoGridScale, gizmoGridScale, gizmoGridScale });

		//ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(gizmoGridTransform), 5.0f);


		if (selectedEntity)
		{
			// Entity transform
			TransformComponent& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			float z = tc.Translation.z;

			bool snap = false; // TODO Input::IsKeyPressed(MAG_KEY_LEFT_CONTROL);
			// TODOfloat snapValue[3] = { m_SnapValue, m_SnapValue, m_SnapValue };
			float snapValues[3] = { 0.1f, 0.1f, 0.1f };

			//if (m_SelectionMode == SelectionMode::Entity)
			{
				ImGuizmo::Manipulate(
					glm::value_ptr(cameraView),
					glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)ImGuizmo::OPERATION::TRANSLATE,
					ImGuizmo::LOCAL,
					glm::value_ptr(transform),
					nullptr,
					snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(transform, translation, rotation, scale);

					if (isOrthographic) translation.z = z;

					glm::vec3 deltaRotation = rotation - tc.Rotation;
					tc.Translation = translation;
					tc.Rotation += deltaRotation;
					tc.Scale = scale;
				}
			}
		}







		ImGui::End();
		ImGui::PopStyleVar();

		return resized;
	}

	


	std::pair<float, float> EditorSceneView::GetMouseViewportSpace()
	{
		auto [mx, my] = ImGui::GetMousePos(); // Input::GetMousePosition();
		mx -= m_Bounds[0].x;
		my -= m_Bounds[0].y;
		auto viewportWidth = m_Bounds[1].x - m_Bounds[0].x;
		auto viewportHeight = m_Bounds[1].y - m_Bounds[0].y;

		return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
	}
}