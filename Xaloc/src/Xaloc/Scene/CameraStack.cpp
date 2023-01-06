#include "xapch.h"
#include "CameraStack.h"

namespace Xaloc {

	/*
	CameraStack::GetPrimaryCamera()
	{
		int priority = std::numeric_limits<int>::max();

		auto group = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : group)
		{
			auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

			if (camera.Priority < priority)
			{
				mainCamera = &camera.Camera;
				cameraTransform = transform.GetTransform();
				priority = camera.Priority;
			}
		}
	}
	*/
}