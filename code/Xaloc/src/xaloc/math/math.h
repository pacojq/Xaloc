#pragma once
#include "xaloc/types.h"
#include <limits>
#include <glm/glm.hpp>


namespace Xaloc
{

#define C_PI = 3.14159265358979323846f;
#define C_RAD2DEG = (180.f / C_PI);
#define C_DEG2RAD = (C_PI / 180.f);

#define C_INT_MIN std::numeric_limits<int>::min()
#define C_INT_MAX std::numeric_limits<int>::max()

#define C_FLOAT_MIN std::numeric_limits<float>::min()
#define C_FLOAT_MAX std::numeric_limits<float>::max()

class ZMath
{
public:

	/// <summary>
	/// Decomposes a given transform into the three passed transform components.
	/// </summary>
	/// <param name="transform">The transform to be decomposed.</param>
	/// <param name="translation">Out translation.</param>
	/// <param name="rotation">Out rotation.</param>
	/// <param name="scale">Out scale.</param>
	/// <returns>Whether the transform has been decomposed successfully.</returns>
	static bool DecomposeTransform(
		const glm::mat4& transform,
		glm::vec3& translation,
		glm::vec3& rotation,
		glm::vec3& scale);
};

}
