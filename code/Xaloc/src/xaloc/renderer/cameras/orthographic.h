#pragma once
#include "xaloc/types.h"
#include "xaloc/core/time.h"
#include "xaloc/renderer/camera.h"
#include <glm/glm.hpp>


namespace Xaloc
{

struct SOrthoCameraBounds
{
	float32 Left;
	float32 Right;
	float32 Bottom;
	float32 Top;

	float32 GetWidth() { return Right - Left; }
	float32 GetHeight() { return Top - Bottom; }
};

class ZOrthographicCamera : public ZCamera
{
public:
	ZOrthographicCamera();

	ZOrthographicCamera(
		uint32 width,
		uint32 height);

	/*
	ZOrthographicCamera(
		float32 left,
		float32 right,
		float32 bottom,
		float32 top,
		float32 zNear = -1.0f,
		float32 zFar = 1.0f);
	*/

	void SetZoom(float32 zoom);

	void SetPixelSize(
		uint32 nWidth,
		uint32 nHeight);

	void SetClipDistance(
		float32 zNear,
		float32 zFar);

	glm::vec2 GetPixelSize() const;

	inline float32 GetZoom() const { return m_Zoom; }
	inline float32 GetNearClip() const { return m_ZNear; }
	inline float32 GetFarClip() const { return m_ZFar; }

	inline const SOrthoCameraBounds& GetBounds() const
	{
		return m_Bounds;
	}

private:
	void RecalculateProjectionMatrix();

	uint32 m_nPixelWidth;
	uint32 m_nPixelHeight;

	float32 m_Zoom = 1.0f;
	float32 m_ZNear = -1.0f;
	float32 m_ZFar = 1.0f;

	SOrthoCameraBounds m_Bounds;
};

}