#pragma once

#include "Xaloc.h"

namespace Xaloc {

	class EditorCamera
	{
	public:
		EditorCamera();
		virtual ~EditorCamera() = default;

		void EditorCamera::SetFocused(bool focused);
		
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		bool OnMouseScroled(MouseScrolledEvent& e);

		SceneCamera* GetCamera() { return &m_CameraData.Camera; }
		const TransformComponent& GetTransform() const { return m_CameraTransform; }
		CameraComponent& GetCameraData() { return m_CameraData; }

	private:
		void CameraZoom(float delta);
		void CameraPan(const glm::vec2& delta);
		void CameraRotate(const glm::vec2& delta);
		
		std::pair<float, float> CalculatePanSpeed() const;

	private:

		TransformComponent m_CameraTransform;
		CameraComponent m_CameraData;

		glm::vec2 m_MousePos = { 0.0f, 0.0f };
		float m_Pitch = 0;
		float m_Yaw = 0;
		bool m_IsTrackingDelta = false;
	};

}