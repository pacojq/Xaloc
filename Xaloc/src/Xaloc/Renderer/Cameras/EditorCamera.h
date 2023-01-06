#pragma once

#include "Xaloc/Core/Timestep.h"
#include "Xaloc/Events/Event.h"
#include "Xaloc/Events/MouseEvent.h"
#include "Xaloc/Renderer/Camera.h"

namespace Xaloc {

	class EditorCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		EditorCamera() = default;
		EditorCamera(const glm::mat4& projectionMatrix);

		virtual void Focus(glm::vec3 focusPoint) = 0;
		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& e) = 0;

		virtual void SetViewportSize(uint32_t width, uint32_t height) = 0;

		virtual glm::vec3 GetUpDirection() = 0;
		virtual glm::vec3 GetRightDirection() = 0;
		virtual glm::vec3 GetForwardDirection() = 0;

		virtual glm::quat GetOrientation() const = 0;

		inline virtual ProjectionType GetProjectionType() const = 0;

		inline virtual float GetGuizmoScale() const = 0;

	public:
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }

		const glm::vec3& GetPosition() const { return m_Position; }


	protected:
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position, m_Rotation;

	};

}