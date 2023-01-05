#pragma once

#include "EditorCamera.h"
#include "OrthographicCamera.h"

#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Xaloc {

	class EditorCameraOrthographic : public EditorCamera
	{
	public:
		EditorCameraOrthographic();

		void Focus(glm::vec3 focusPoint) override;
		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;
		

		inline void SetViewportSize(uint32_t width, uint32_t height) override 
		{ 
			m_ViewportWidth = width;
			m_ViewportHeight = height;
			SetAspectRatio((float)width / (float)height); 
		}

		inline glm::vec3 GetUpDirection() override { return glm::vec3(0.0f, 1.0f, 0.0f); }
		inline glm::vec3 GetRightDirection() override { return glm::vec3(1.0f, 0.0f, 0.0f); }
		inline glm::vec3 GetForwardDirection() override { return glm::vec3(0.0f, 0.0f, -1.0f); }

		inline glm::quat GetOrientation() const override { return glm::quat(m_Rotation); }

		inline virtual ProjectionType GetProjectionType() const override { return ProjectionType::Orthographic; }

		inline virtual float GetGuizmoScale() const override { return 0.1f * m_OrthoSize; }

	private:
		void SetAspectRatio(float aspectRatio);

		void UpdateCameraView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseZoom(float delta);

		std::pair<float, float> PanSpeed() const;
		inline float RotationSpeed() const { return 0.8f; }
		float ZoomSpeed() const;


	private:
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