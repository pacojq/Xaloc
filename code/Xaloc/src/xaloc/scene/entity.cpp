#include "xapch.h"
#include "xaloc/scene/entity.h"
#include "xaloc/scene/scene.h"

namespace Xaloc
{

#if DEPRECATED
void ZEntity::SetTransform(glm::mat4 trans)
{
	auto& transformComponent = GetComponent<CmpTransform>();

	glm::vec3 scale, translation, skew;
	glm::vec4 perspective;
	glm::quat orientation;
	glm::decompose(trans, scale, orientation, translation, skew, perspective);

	if (glm::abs(orientation.y < 0.001f))
	{
		orientation.y = 0.0f;
	}

	transformComponent.Translation = translation;
	transformComponent.Rotation = glm::degrees(glm::eulerAngles(orientation));
	transformComponent.Scale = scale;
}
#endif

}