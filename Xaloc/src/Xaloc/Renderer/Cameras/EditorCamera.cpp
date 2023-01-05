#include "xapch.h"
#include "EditorCamera.h"


namespace Xaloc {

	EditorCamera::EditorCamera(const glm::mat4& projectionMatrix)
		: Camera(projectionMatrix),
		m_Rotation(glm::vec3(90.0f, 0.0f, 0.0f))
	{
	}

}