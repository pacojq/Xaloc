#pragma once

#include "Xaloc/Renderer/OrthographicCamera.h"
#include "Xaloc/Core/Timestep.h"

#include "Xaloc/Events/ApplicationEvent.h"
#include "Xaloc/Events/MouseEvent.h"

namespace Xaloc {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};


	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const  { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }

		const OrthographicCameraBounds& GetBounds() { return m_Bounds; }

	private:
		void CalculateView();

		bool OnMouseScroled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);


	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 3.0f;
		float m_CameraRotationSpeed = 50.0f;

	};
}