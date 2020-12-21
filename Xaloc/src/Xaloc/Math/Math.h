#pragma once

#include <glm/glm.hpp>

namespace Xaloc::Math {

	/// <summary>
	/// Decomposes a given transform into the three passed transform components.
	/// </summary>
	/// <param name="transform">The transform to be decomposed.</param>
	/// <param name="translation">Out translation.</param>
	/// <param name="rotation">Out rotation.</param>
	/// <param name="scale">Out scale.</param>
	/// <returns>Whether the transform has been decomposed successfully.</returns>
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

}
