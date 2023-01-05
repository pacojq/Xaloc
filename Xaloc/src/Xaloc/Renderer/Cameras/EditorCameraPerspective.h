#pragma once

#include "EditorCamera.h"

namespace Xaloc {

	class EditorCameraPerspective : public EditorCamera
	{
	public:
		EditorCameraPerspective() = default;
		EditorCameraPerspective(const glm::mat4& projectionMatrix);

		void Focus(glm::vec3 focusPoint) override;
		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;

		inline void SetViewportSize(uint32_t width, uint32_t height) override { m_ViewportWidth = width; m_ViewportHeight = height; }


		glm::vec3 GetUpDirection() override;
		glm::vec3 GetRightDirection() override;
		glm::vec3 GetForwardDirection() override;

		glm::quat GetOrientation() const override;

		inline virtual ProjectionType GetProjectionType() const override {return ProjectionType::Perspective; }

		inline virtual float GetGuizmoScale() const override { return m_Distance / focusSafeDistance; }

	public:
		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

	private:
		void UpdateCameraView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition();

		std::pair<float, float> PanSpeed() const;
		inline float RotationSpeed() const { return 0.8f; }
		float ZoomSpeed() const;


	private:
		glm::vec3 m_FocalPoint;

		bool m_Panning, m_Rotating;
		glm::vec2 m_InitialMousePosition;
		glm::vec3 m_InitialFocalPoint, m_InitialRotation;

		float m_Distance;
		float m_Pitch, m_Yaw;

		uint32_t m_ViewportWidth = 1280, m_ViewportHeight = 720;

		float focusSafeDistance = 100.f;
	};

}