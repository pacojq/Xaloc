#pragma once

#include "Xaloc.h"

namespace Xaloc {

	class EditorCamera : public Layer
	{
	public:
		EditorCamera();
		virtual ~EditorCamera() = default;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;

		bool OnMouseScroled(MouseScrolledEvent& e);

		const Ref<Camera>& GetCamera() const { return m_Camera; }
		const TransformComponent& GetTransform() const { return m_CameraTransform; }
		PerspectiveCameraDataComponent& GetCameraData() { return m_CameraData; }

	private:
		void CameraZoom(float delta);
		void CameraPan(const glm::vec2& delta);
		void CameraRotate(const glm::vec2& delta);
		
		std::pair<float, float> CalculatePanSpeed() const;

	private:

		Ref<Camera> m_Camera;
		TransformComponent m_CameraTransform;
		PerspectiveCameraDataComponent m_CameraData;

		glm::vec2 m_MousePos = { 0.0f, 0.0f };
		float m_Pitch = 0;
		float m_Yaw = 0;
		bool m_IsTrackingDelta = false;
	};

}