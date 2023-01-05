#include "xapch.h"
#include "OrthographicCamera.h"

#include "Xaloc/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Xaloc {

	OrthographicCamera::OrthographicCamera()
		: m_PixelSize(Renderer2D::PX_PER_UNIT, Renderer2D::PX_PER_UNIT),
		  m_Zoom(1.0f),
		  m_Bounds({ -0.5f, 0.5f, -0.5f, 0.5f }),
		  Camera(glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, m_ZNear, m_ZFar))
	{

	}

	OrthographicCamera::OrthographicCamera(float width, float height)
		: m_PixelSize(width, height),
		  m_Zoom(1.0f),
		  m_Bounds({ -0.5f, 0.5f, -0.5f, 0.5f }),
		  Camera(glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, m_ZNear, m_ZFar))
	{
		RecalculateProjectionMatrix();
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
		: m_PixelSize(right - left, top - bottom),
		  m_Zoom(1.0f),
		  m_Bounds({ left, right, bottom, top }),
		  Camera(glm::ortho(left, right, bottom, top, zNear, zFar))
	{
	}




	void OrthographicCamera::SetZoom(float zoom)
	{
		m_Zoom = zoom;
		RecalculateProjectionMatrix();
	}


	void OrthographicCamera::SetPixelSize(float width, float height)
	{
		m_PixelSize.x = width;
		m_PixelSize.y = height;
		RecalculateProjectionMatrix();
	}


	void OrthographicCamera::SetClipDistance(float zNear, float zFar)
	{
		m_ZNear = zNear;
		m_ZFar = zFar;
		RecalculateProjectionMatrix();
	}


	void OrthographicCamera::RecalculateProjectionMatrix()
	{
		const float hw = ((m_PixelSize.x / m_Zoom) * 0.5f) / Renderer2D::PX_PER_UNIT;
		const float hh = ((m_PixelSize.y / m_Zoom) * 0.5f) / Renderer2D::PX_PER_UNIT;

		m_Bounds = { -hw, hw, -hh, hh };
		m_ProjectionMatrix = glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, m_ZNear, m_ZFar);
	}

	/*
	* 
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
	* 
	* 
	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 i = glm::mat4(1.0f); // identity
		glm::mat4 transform = glm::translate(i, m_Position) * glm::rotate(i, glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	*/

}