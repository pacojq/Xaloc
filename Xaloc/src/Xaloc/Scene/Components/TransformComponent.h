#pragma once

#include <glm/glm.hpp>

namespace Xaloc {

	struct TransformComponent
	{
		glm::mat4 Transform;

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

}