#pragma once
#include <glm/glm.hpp>


namespace Xaloc
{

/// <summary>
/// Axis-aligned bounding box
/// </summary>
struct SBoundingBox
{
	glm::vec3 Min;
	glm::vec3 Max;

	SBoundingBox()
		: Min(0.0f)
		, Max(0.0f)
	{
	}

	SBoundingBox(const glm::vec3& min, const glm::vec3& max)
		: Min(min)
		, Max(max)
	{
	}

};

}