#pragma once

#include <glm/glm.hpp>

namespace Xaloc {

	class Camera
	{
	public:
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}

		const glm::mat4& GetProjection() const { return m_Projection; }
		const void SetProjection(const glm::mat4& proj) { m_Projection = proj; }
	private:
		glm::mat4 m_Projection;
	};

}