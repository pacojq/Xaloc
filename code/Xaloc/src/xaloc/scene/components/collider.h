#pragma once
#include <glm/glm.hpp>


namespace Xaloc
{

struct CmpCollider
{
	glm::vec2 Size;

	operator glm::vec2&() { return Size; }
	operator const glm::vec2&() const { return Size; }
};

}