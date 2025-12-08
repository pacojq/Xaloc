#pragma once
#include <glm/glm.hpp>

namespace Xaloc
{

class ZCamera
{
public:
	ZCamera() = default;

	ZCamera(const glm::mat4& projection)
		: m_ProjectionMatrix(projection)
	{
	}

	virtual ~ZCamera() = default;

	const glm::mat4& GetProjection() const
	{
		return m_ProjectionMatrix;
	}

protected:
	glm::mat4 m_ProjectionMatrix;
};

}