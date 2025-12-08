#pragma once
#include "xaloc/scene/components.h"
#include "xaloc/renderer/cameras/orthographic.h"
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>


namespace Xaloc
{

struct SSceneCamera
{
	CmpTransform Transform;

	ZOrthographicCamera Camera;
	int32 nPriority = 0; // TODO: The lower the priority, the more possible it will be our main camera
	bool bOverrideRenderTarget = false; // TODO

	// TODO: aspect ratio, zoom and all stuff

	SSceneCamera() = default;
	SSceneCamera(const SSceneCamera&) = default;

	SSceneCamera(uint32 nPixelWidth, uint32 nPixelHeight)
		: Camera(nPixelWidth, nPixelHeight)
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

class ZCameraStack
{
public:
	ZCameraStack() = default;
	~ZCameraStack() = default;

	// TODO: camera stack dunctionality
	const SSceneCamera& ReadCamera(int32 nCamera) const;
	SSceneCamera& WriteCamera(int32 nCamera);

	SSceneCamera& MainCamera() { return m_MainCamera; }

private:
	SSceneCamera m_MainCamera;

	// TODO: std::vector<SSceneCamera> m_aCameras;
};

}