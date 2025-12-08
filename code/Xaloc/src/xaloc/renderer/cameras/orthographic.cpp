#include "xapch.h"
#include "xaloc/renderer/cameras/orthographic.h"
#include "xaloc/renderer/renderer2D.h"
#include <glm/gtc/matrix_transform.hpp>


namespace Xaloc
{

ZOrthographicCamera::ZOrthographicCamera()
	: ZOrthographicCamera(
		(uint32)C_PIXELS_PER_UNIT,
		(uint32)C_PIXELS_PER_UNIT)
{
}

ZOrthographicCamera::ZOrthographicCamera(
	uint32 nWidth,
	uint32 nHeight)
	: m_nPixelWidth(nWidth)
	, m_nPixelHeight(nHeight)
	, m_Zoom(1.0f)
	, ZCamera()
{
	RecalculateProjectionMatrix();
}

/*
ZOrthographicCamera::ZOrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
	: m_PixelSize(right - left, top - bottom),
		m_Zoom(1.0f),
		m_Bounds({ left, right, bottom, top }),
		ZCamera(glm::ortho(left, right, bottom, top, zNear, zFar))
{
}
*/

void ZOrthographicCamera::SetZoom(float32 zoom)
{
	m_Zoom = zoom;
	RecalculateProjectionMatrix();
}

void ZOrthographicCamera::SetPixelSize(
	uint32 nWidth,
	uint32 nHeight)
{
	m_nPixelWidth = nWidth;
	m_nPixelHeight = nHeight;
	RecalculateProjectionMatrix();
}

void ZOrthographicCamera::SetClipDistance(float32 zNear, float32 zFar)
{
	m_ZNear = zNear;
	m_ZFar = zFar;
	RecalculateProjectionMatrix();
}

glm::vec2 ZOrthographicCamera::GetPixelSize() const
{
	return { float32(m_nPixelWidth), float32(m_nPixelHeight) };
}

void ZOrthographicCamera::RecalculateProjectionMatrix()
{
	const float32 fHalfWidth = ((m_nPixelWidth / m_Zoom) * 0.5f) / C_PIXELS_PER_UNIT;
	const float32 fHalfHeight = ((m_nPixelHeight / m_Zoom) * 0.5f) / C_PIXELS_PER_UNIT;

	m_Bounds = { -fHalfWidth, fHalfWidth, -fHalfHeight, fHalfHeight };
	m_ProjectionMatrix = glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, m_ZNear, m_ZFar);
}

}