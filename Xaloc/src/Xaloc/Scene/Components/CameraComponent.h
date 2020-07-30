#pragma once

#include "Xaloc/Renderer/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Xaloc {
	
	struct CameraComponent
	{
		Camera Camera;
		int Priority = 0; // The lower the priority, the more possible it will be our main camera

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			: Camera(projection) {}
	};


	struct OrthographicCameraDataComponent
	{
		float Width = 1280.0f;
		float Height = 720.0f;
		float Zoom = 1.0f;
		float ZNear = -1.0f;
		float ZFar = 1.0f;

		glm::mat4 CalculateProjectionMatrix()
		{
			float aspectRatio = Width / Height;
			return glm::ortho(-aspectRatio * Zoom, aspectRatio * Zoom, -Zoom, Zoom, ZNear, ZFar);
		}
	};

	
	struct PerspectiveCameraDataComponent
	{
		float Width = 1280.0f;
		float Height = 720.0f;
		float Fov = 45.0f;
		float ZNear = 0.1f;
		float ZFar = 10000.f;

		glm::mat4 CalculateProjectionMatrix()
		{
			float aspectRatio = Width / Height;
			//return glm::ortho(-aspectRatio * 5.0f, aspectRatio * 5.0f, -5.0f, 5.0f, -1.0f, 1.0f);
			return glm::perspectiveFov(glm::radians(Fov), Width, Height, ZNear, ZFar);
		}
	};
	
}
