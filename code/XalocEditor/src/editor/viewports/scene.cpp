#include "xapch.h"
#include "editor/viewports/scene.h"
#include "xaloc/math/math.h"
#include "xaloc/scene/components.h"
#include "xaloc/renderer/cameras/editor.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "tooling/imgui/imguizmo.h"

using namespace Xaloc;


namespace Editor
{

static const float TOOLBAR_HEIGHT = 32.0f;

ZEditorSceneView::ZEditorSceneView(const std::string& name)
	: m_Name(name)
{
}

bool ZEditorSceneView::OnImGuiDraw(
	const ZFramebuffer* pFramebuffer,
	const ZRef<ZEditorCamera>& camera,
	const ZEntity& selectedEntity)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin(m_Name.c_str());

	m_Focused = ImGui::IsWindowFocused();
	m_Hovered = ImGui::IsWindowHovered();

	auto viewportOffset = ImGui::GetCursorPos();
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();

	bool bResized = false;
	if (m_Size != *((glm::vec2*)&viewportSize))
	{
		m_Size = { viewportSize.x, viewportSize.y };
		bResized = true;
	}

	auto windowSize = ImGui::GetWindowSize();
	ImVec2 minBound = ImGui::GetWindowPos();
	minBound.x += viewportOffset.x;
	minBound.y += viewportOffset.y;

	ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };

	m_Bounds[0] = { minBound.x, minBound.y };
	m_Bounds[1] = { maxBound.x, maxBound.y };

	// TODO m_AllowViewportCameraEvents = ImGui::IsMouseHoveringRect(minBound, maxBound);

	const uint32 texID = pFramebuffer->GetColorAttachmentRendererID();
	//uint32_t texID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)texID, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	// GUIZMO

	// Editor Camera
	const glm::mat4& cameraProjection = camera->GetProjection();
	glm::mat4 cameraView = camera->GetViewMatrix();

	float rw = (float)viewportSize.x;
	float rh = (float)viewportSize.y;
	ImGuizmo::SetOrthographic(true);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(minBound.x, minBound.y, rw, rh);

	// Guizmo grid
	//float gizmoGridScale = 1.0f;// + glm::log(m_EditorCamera.GetDistance());
	//glm::mat4 gizmoGridTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
	//	* glm::rotate(glm::mat4(1.0f), 0.0f, { 0.0f, 0.0f, 1.0f })
	//	* glm::scale(glm::mat4(1.0f), { gizmoGridScale, gizmoGridScale, gizmoGridScale });

	//ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(gizmoGridTransform), 5.0f);

	if (selectedEntity && selectedEntity.HasComponent<CmpTransform>())
	{
		// Entity transform
		CmpTransform& tc = selectedEntity.GetComponent<CmpTransform>();
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
				ZMath::DecomposeTransform(transform, translation, rotation, scale);

				translation.z = z;

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();

	return bResized;
}

std::pair<float, float> ZEditorSceneView::GetMouseViewportSpace()
{
	auto [mx, my] = ImGui::GetMousePos(); // Input::GetMousePosition();
	mx -= m_Bounds[0].x;
	my -= m_Bounds[0].y;
	auto viewportWidth = m_Bounds[1].x - m_Bounds[0].x;
	auto viewportHeight = m_Bounds[1].y - m_Bounds[0].y;

	return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
}

}