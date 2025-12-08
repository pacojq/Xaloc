#include "xapch.h"
#include "xaloc/renderer/cameras/editor.h"
#include "xaloc/core/input/input.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// TODO: move this to editor side

namespace Xaloc
{

ZEditorCamera::ZEditorCamera()
	: m_OrthoSize(10.0f),
	m_AspectRatio(1.0f),
	m_Bounds({ -m_AspectRatio * m_OrthoSize * 0.5f, m_AspectRatio * m_OrthoSize * 0.5f, -m_OrthoSize * 0.5f, m_OrthoSize * 0.5f }),
	m_Rotation(glm::vec3(90.0f, 0.0f, 0.0f)),
	ZCamera(glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, m_ZNear, m_ZFar))
{
	m_Position = glm::vec3(0.0f, 0.0f, -100.0f);
	m_Rotation = glm::vec3(90.0f, 0.0f, 0.0f);

	UpdateCameraView();
}

void ZEditorCamera::SetAspectRatio(float aspectRatio)
{
	m_AspectRatio = aspectRatio;
	m_Bounds = { -m_AspectRatio * m_OrthoSize * 0.5f, m_AspectRatio * m_OrthoSize * 0.5f, -m_OrthoSize * 0.5f, m_OrthoSize * 0.5f };
	m_ProjectionMatrix = glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, m_ZNear, m_ZFar);
}

void ZEditorCamera::UpdateCameraView()
{
	//glm::quat orientation = GetOrientation();
	//m_Rotation = glm::eulerAngles(orientation) * (180.0f / (float)C_PI);
	m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position);// *glm::toMat4(orientation);
	m_ViewMatrix = glm::inverse(m_ViewMatrix);
}

void ZEditorCamera::Focus(glm::vec3 focusPoint)
{
	m_Position = focusPoint;
	UpdateCameraView();
}

void ZEditorCamera::OnUpdate(ZTimestep ts)
{
	const glm::vec2& mouse{ ZInput::GetMouseX(), ZInput::GetMouseY() };
	glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
	m_InitialMousePosition = mouse;

	if (ZInput::IsMouseButtonPressed(XA_MOUSE_BUTTON_MIDDLE))
	{
		MousePan(delta);
	}
	/*
	if (Input::IsKeyPressed(XA_KEY_A))
		m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
	if (Input::IsKeyPressed(XA_KEY_D))
		m_CameraPosition.x += m_CameraTranslationSpeed * ts;
	if (Input::IsKeyPressed(XA_KEY_W))
		m_CameraPosition.y += m_CameraTranslationSpeed * ts;
	if (Input::IsKeyPressed(XA_KEY_S))
		m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
	*/

	UpdateCameraView();
}

void ZEditorCamera::OnEvent(ZEvent& rEvent)
{
	ZEventDispatcher dispatcher(rEvent);
	dispatcher.Dispatch<EvtMouseScrolled>(XA_BIND_EVENT_FN(ZEditorCamera::OnMouseScroll));
}

bool ZEditorCamera::OnMouseScroll(EvtMouseScrolled& e)
{
	float delta = e.GetYOffset();
	MouseZoom(delta);
	UpdateCameraView();
	return false;
}

std::pair<float, float> ZEditorCamera::PanSpeed() const
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

float ZEditorCamera::ZoomSpeed() const
{
	float size = glm::pow(m_OrthoSize * 0.5f, 1.2f);
	size = std::max(size, 0.1f);
	float speed = size * 0.2f;
	speed = std::min(speed, 20.0f); // max speed = 100
	return speed;
}

void ZEditorCamera::MousePan(const glm::vec2& delta)
{
	auto [xSpeed, ySpeed] = PanSpeed();
	m_Position += -GetRightDirection() * delta.x * xSpeed;// *m_Distance;
	m_Position += GetUpDirection() * delta.y * ySpeed;// *m_Distance;

	UpdateCameraView();
}

void ZEditorCamera::MouseZoom(float delta)
{
	m_OrthoSize -= delta * ZoomSpeed();
	m_OrthoSize = std::max(m_OrthoSize, 0.1f);

	m_Bounds = { -m_AspectRatio * m_OrthoSize * 0.5f, m_AspectRatio * m_OrthoSize * 0.5f, -m_OrthoSize * 0.5f, m_OrthoSize * 0.5f };
	m_ProjectionMatrix = glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, m_ZNear, m_ZFar);
}

}