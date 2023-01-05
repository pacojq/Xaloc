#include "xapch.h"
#include "EditorCameraPerspective.h"

#include "Xaloc/Core/Input/Input.h"
#include "Xaloc/Core/Input/MouseButtonCodes.h"

#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


#define M_PI 3.14159f

namespace Xaloc {

	EditorCameraPerspective::EditorCameraPerspective(const glm::mat4& projectionMatrix)
		: EditorCamera(projectionMatrix)
	{
		m_Rotation = glm::vec3(90.0f, 0.0f, 0.0f);
		m_FocalPoint = glm::vec3(0.0f);

		glm::vec3 position = { -5, 5, 5 };
		m_Distance = glm::distance(position, m_FocalPoint);

		m_Yaw = 3.0f * (float)M_PI / 4.0f;
		m_Pitch = M_PI / 4.0f;

		UpdateCameraView();
	}



	void EditorCameraPerspective::UpdateCameraView()
	{
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_Rotation = glm::eulerAngles(orientation) * (180.0f / (float)M_PI);
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	void EditorCameraPerspective::Focus(glm::vec3 focusPoint)
	{
		m_FocalPoint = focusPoint;
		if (focusSafeDistance < m_Distance)
		{
			float delta = m_Distance - focusSafeDistance;
			MouseZoom(delta / ZoomSpeed());
			UpdateCameraView();
		}

	}

	void EditorCameraPerspective::OnUpdate(Timestep ts)
	{
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;


		if (Input::IsMouseButtonPressed(XA_MOUSE_BUTTON_MIDDLE))
			MousePan(delta);
		else if (Input::IsMouseButtonPressed(XA_MOUSE_BUTTON_LEFT))
			MouseRotate(delta);
		else if (Input::IsMouseButtonPressed(XA_MOUSE_BUTTON_RIGHT))
			MouseZoom(delta.y);

		UpdateCameraView();
	}

	void EditorCameraPerspective::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(XA_BIND_EVENT_FN(EditorCameraPerspective::OnMouseScroll));
	}

	bool EditorCameraPerspective::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateCameraView();
		return false;
	}







	std::pair<float, float> EditorCameraPerspective::PanSpeed() const
	{
		static const float MAX_PAN = 2.4f;

		float x = std::min(m_ViewportWidth / 1000.0f, MAX_PAN); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, MAX_PAN); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCameraPerspective::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}




	void EditorCameraPerspective::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCameraPerspective::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCameraPerspective::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCameraPerspective::GetUpDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCameraPerspective::GetRightDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCameraPerspective::GetForwardDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCameraPerspective::CalculatePosition()
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	glm::quat EditorCameraPerspective::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}
}