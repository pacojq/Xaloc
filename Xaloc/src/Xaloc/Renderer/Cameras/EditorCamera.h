#pragma once

#include "Xaloc/Core/Timestep.h"
#include "Xaloc/Events/Event.h"
#include "Xaloc/Events/MouseEvent.h"
#include "Xaloc/Renderer/Camera.h"

#include "OrthographicCamera.h"

#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Xaloc {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera();
		~EditorCamera() override = default;

		void Focus(glm::vec3 focusPoint);
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline void SetViewportSize(uint32_t width, uint32_t height)
		{
			m_ViewportWidth = width;
			m_ViewportHeight = height;
			SetAspectRatio((float)width / (float)height);
		}

		inline glm::vec3 GetUpDirection() { return glm::vec3(0.0f, 1.0f, 0.0f); }
		inline glm::vec3 GetRightDirection() { return glm::vec3(1.0f, 0.0f, 0.0f); }
		inline glm::vec3 GetForwardDirection() { return glm::vec3(0.0f, 0.0f, -1.0f); }

		inline glm::quat GetOrientation() const { return glm::quat(m_Rotation); }

		inline virtual float GetGuizmoScale() const { return 0.1f * m_OrthoSize; }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }

		const glm::vec3& GetPosition() const { return m_Position; }


	private:
		void SetAspectRatio(float aspectRatio);

		void UpdateCameraView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseZoom(float delta);

		std::pair<float, float> PanSpeed() const;
		inline float RotationSpeed() const { return 0.8f; }
		float ZoomSpeed() const;


	protected:
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position, m_Rotation;

		float m_OrthoSize = 1.0f;
		float m_AspectRatio = 1.0f;
		OrthographicCameraBounds m_Bounds;
		float m_ZNear = -500.0f;
		float m_ZFar = 1000.0f;

		bool m_Panning;
		glm::vec2 m_InitialMousePosition;

		uint32_t m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};

}