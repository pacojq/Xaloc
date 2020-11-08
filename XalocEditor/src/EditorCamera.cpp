#include "xapch.h"
#include "EditorCamera.h"

namespace Xaloc {

	EditorCamera::EditorCamera()
	{
		m_CameraData = {};
		m_CameraData.Camera.SetViewportSize(1280.0, 720.0);
		m_CameraData.Camera.SetPerspectiveVerticalFOV(45.0f);
		m_CameraData.Camera.SetPerspectiveNearClip(0.01f);
		m_CameraData.Camera.SetPerspectiveFarClip(1000.0f);
		m_CameraData.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
		
		m_CameraTransform = {};
		m_CameraTransform.Transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 10.0f })
			* glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f });
	}


	void EditorCamera::SetFocused(bool focused)
	{
		m_IsTrackingDelta = focused;
		m_MousePos = { Input::GetMouseX(), Input::GetMouseY() };
	}

	
	void EditorCamera::OnUpdate(Timestep ts)
	{
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		//glm::vec2 delta = (mouse - m_MousePos) * 0.003f;
		glm::vec2 delta = (mouse - m_MousePos) * 6.5f * ts.GetSeconds();
		
		if (Input::IsMouseButtonPressed(XA_MOUSE_BUTTON_MIDDLE))
		{
			if (!m_IsTrackingDelta)
				delta = { 0.0f, 0.0f };
			m_IsTrackingDelta = true;
			
			CameraPan(delta);
		}
		else if (Input::IsMouseButtonPressed(XA_MOUSE_BUTTON_LEFT))
		{
			if (!m_IsTrackingDelta)
				delta = { 0.0f, 0.0f };
			m_IsTrackingDelta = true;
			
			CameraRotate(delta);
		}
		else
		{
			m_IsTrackingDelta = false;
		}
		
		m_MousePos = mouse;
	}


	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(XA_BIND_EVENT_FN(EditorCamera::OnMouseScroled));
	}


	bool EditorCamera::OnMouseScroled(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.5f; // TODO relative to distance to selected object?
		CameraZoom(delta);
		return false;
	}


	void EditorCamera::CameraZoom(float delta)
	{
		auto [translation, rotationQuat, scale] = GetTransformDecomposition(m_CameraTransform);

		glm::vec3 forward = glm::rotate(rotationQuat, glm::vec3(0.0f, 0.0f, -delta));
		translation += forward;

		m_CameraTransform.Transform = glm::translate(glm::mat4(1.0f), translation) *
			glm::toMat4(rotationQuat) *
			glm::scale(glm::mat4(1.0f), scale);

		// TODO recalculate
		//m_CameraData.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
	}

	void EditorCamera::CameraPan(const glm::vec2& delta)
	{
		auto [translation, rotationQuat, scale] = GetTransformDecomposition(m_CameraTransform);

		auto [xSpd, ySpd] = CalculatePanSpeed();

		float dx = delta.x * xSpd;
		glm::vec3 right = glm::rotate(rotationQuat, glm::vec3(-dx, 0.0f, 0.0f));
		translation += right;

		float dy = delta.y * ySpd;
		glm::vec3 up = glm::rotate(rotationQuat, glm::vec3(0.0f, dy, 0.0f));
		translation += up;

		m_CameraTransform.Transform = glm::translate(glm::mat4(1.0f), translation) *
			glm::toMat4(rotationQuat) *
			glm::scale(glm::mat4(1.0f), scale);

		// TODO recalculate
		//m_CameraData.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
	}

	void EditorCamera::CameraRotate(const glm::vec2& delta)
	{
		auto [translation, rotationQuat, scale] = GetTransformDecomposition(m_CameraTransform);
				
		float rotSpd = 0.8f;
		glm::vec3 up = glm::rotate(rotationQuat, glm::vec3(0.0f, 1.0f, 0.0f));
		
		float yawSign = up.y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * rotSpd;
		m_Pitch += delta.y * rotSpd;

		glm::vec3 euler = glm::vec3(-m_Pitch, -m_Yaw, 0.0f);
		
		m_CameraTransform.Transform = glm::translate(glm::mat4(1.0f), translation) *
			glm::toMat4(glm::quat(glm::radians(euler))) *
			glm::scale(glm::mat4(1.0f), scale);

		// TODO recalculate
		m_CameraData.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
	}
	
	

	std::pair<float, float> EditorCamera::CalculatePanSpeed() const
	{
		// TODO get camera width and height
		float width = 1280.0f;
		float height = 720.0f;
		
		float x = std::min(width / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(height / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}
}