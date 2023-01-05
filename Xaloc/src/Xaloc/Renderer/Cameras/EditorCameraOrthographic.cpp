#include "xapch.h"
#include "EditorCameraOrthographic.h"

#include "Xaloc/Core/Input/Input.h"
#include "Xaloc/Core/Input/MouseButtonCodes.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


#define M_PI 3.14159f

namespace Xaloc {

	EditorCameraOrthographic::EditorCameraOrthographic()
		: m_OrthoSize(10.0f),
		  m_AspectRatio(1.0f),
		  m_Bounds({ -m_AspectRatio * m_OrthoSize * 0.5f, m_AspectRatio * m_OrthoSize * 0.5f, -m_OrthoSize * 0.5f, m_OrthoSize * 0.5f }),
		  EditorCamera(glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, m_ZNear, m_ZFar))
	{
		m_Position = glm::vec3(0.0f, 0.0f, -100.0f);
		m_Rotation = glm::vec3(90.0f, 0.0f, 0.0f);

		UpdateCameraView();
	}


	void EditorCameraOrthographic::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
		m_Bounds = { -m_AspectRatio * m_OrthoSize * 0.5f, m_AspectRatio * m_OrthoSize * 0.5f, -m_OrthoSize * 0.5f, m_OrthoSize * 0.5f };
		m_ProjectionMatrix = glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, m_ZNear, m_ZFar);
	}


	void EditorCameraOrthographic::UpdateCameraView()
	{
		//glm::quat orientation = GetOrientation();
		//m_Rotation = glm::eulerAngles(orientation) * (180.0f / (float)M_PI);
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position);// *glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	void EditorCameraOrthographic::Focus(glm::vec3 focusPoint)
	{
		m_Position = focusPoint;
		UpdateCameraView();
	}




	void EditorCameraOrthographic::OnUpdate(Timestep ts)
	{
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;

		if (Input::IsMouseButtonPressed(XA_MOUSE_BUTTON_MIDDLE))
			MousePan(delta);

		/*
		if (Input::IsKeyPressed(MAG_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(MAG_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(MAG_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(MAG_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		*/

		UpdateCameraView();
	}

	void EditorCameraOrthographic::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(XA_BIND_EVENT_FN(EditorCameraOrthographic::OnMouseScroll));
	}

	bool EditorCameraOrthographic::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset();
		MouseZoom(delta);
		UpdateCameraView();
		return false;
	}







	std::pair<float, float> EditorCameraOrthographic::PanSpeed() const
	{
		static const float MAX_PAN = 2.4f;

		float x = std::min(m_ViewportWidth / 1000.0f, MAX_PAN); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, MAX_PAN); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		float spd = m_OrthoSize * 1.4f;
		xFactor *= spd;
		yFactor *= spd;

		return { xFactor, yFactor };
	}

	float EditorCameraOrthographic::ZoomSpeed() const
	{
		float size = glm::pow(m_OrthoSize * 0.5f, 1.2f);
		size = std::max(size, 0.1f);
		float speed = size * 0.2f;
		speed = std::min(speed, 20.0f); // max speed = 100
		return speed;
	}




	void EditorCameraOrthographic::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_Position += -GetRightDirection() * delta.x * xSpeed;// *m_Distance;
		m_Position += GetUpDirection() * delta.y * ySpeed;// *m_Distance;

		UpdateCameraView();
	}

	void EditorCameraOrthographic::MouseZoom(float delta)
	{
		m_OrthoSize -= delta * ZoomSpeed();
		m_OrthoSize = std::max(m_OrthoSize, 0.1f);

		m_Bounds = { -m_AspectRatio * m_OrthoSize * 0.5f, m_AspectRatio * m_OrthoSize * 0.5f, -m_OrthoSize * 0.5f, m_OrthoSize * 0.5f };
		m_ProjectionMatrix = glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, m_ZNear, m_ZFar);
	}
}