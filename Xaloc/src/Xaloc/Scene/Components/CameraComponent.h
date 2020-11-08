#pragma once

#include "Xaloc/Scene/SceneCamera.h"

namespace Xaloc {
	
	struct CameraComponent
	{
		SceneCamera Camera;
		int Priority = 0; // The lower the priority, the more possible it will be our main camera
		bool FixedAspectRatio = false; // TODO

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
	
}
