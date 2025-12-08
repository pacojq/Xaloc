#pragma once
#include <glm/glm.hpp>

namespace Xaloc {
	class ZScene;
	class ZEntity;
}

namespace Xaloc
{	

class ZPhysicSpace
{
public:
	template<typename T>
	bool PlaceMeeting(const ZEntity& entity);

	template<typename T>
	bool PlaceMeeting(const glm::vec3& position, const ZEntity& entity);
};

}