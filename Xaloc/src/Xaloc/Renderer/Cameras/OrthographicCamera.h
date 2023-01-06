#pragma once

#include "Xaloc/Core/Timestep.h"
#include "Xaloc/Renderer/Camera.h"
#include <glm/glm.hpp>

namespace Xaloc {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};


	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera();
		OrthographicCamera(float width, float height);
		OrthographicCamera(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);


		inline void SetViewportSize(uint32_t width, uint32_t height) { SetPixelSize(width, height); }

		void SetZoom(float zoom);
		void SetPixelSize(float width, float height);
		void SetClipDistance(float zNear, float zFar);

		inline glm::vec2 GetPixelSize() const { return m_PixelSize; }
		inline float GetZoom() const { return m_Zoom; }
		inline float GetNearClip() const { return m_ZNear; }
		inline float GetFarClip() const { return m_ZFar; }

		inline const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

	private:
		void RecalculateProjectionMatrix();

	private:
		float m_Zoom = 1.0f;
		glm::vec2 m_PixelSize;

		OrthographicCameraBounds m_Bounds;
		float m_ZNear = -1.0f;
		float m_ZFar = 1.0f;
	};
}