#pragma once

#include <glm/glm.hpp>

namespace Xaloc {

	struct ColliderComponent
	{
		glm::vec2 Size;

		operator glm::vec2&() { return Size; }
		operator const glm::vec2&() const { return Size; }
	};

}