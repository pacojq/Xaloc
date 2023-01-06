#pragma once

#include "Components.h"
#include "Xaloc/Renderer/Cameras/OrthographicCamera.h"

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>

namespace Xaloc {

	struct SceneCamera
	{
		TransformComponent Transform;

		OrthographicCamera Camera;
		int Priority = 0; // TODO: The lower the priority, the more possible it will be our main camera
		bool OverrideRenderTarget = false; // TODO

		// TODO: aspect ratio, zoom and all stuff

		SceneCamera() = default;
		SceneCamera(const SceneCamera&) = default;

		SceneCamera(uint32_t pixelWidth, uint32_t pixelHeight)
			: Camera(pixelWidth, pixelHeight)
		{

		}

		// TODO: active cameras
		inline bool IsActive() const { return true; }


		inline glm::mat4 ViewProjectionMatrix() const
		{
			//     Projection matrix      * View matrix
			return Camera.GetProjection() * glm::inverse(Transform.GetTransform());
		}
	};


	class CameraStack
	{
	public:
		CameraStack() = default;
		~CameraStack() = default;

		SceneCamera& MainCamera() { return m_MainCamera; }


	private:
		SceneCamera m_MainCamera;
	};

}