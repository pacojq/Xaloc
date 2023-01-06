#pragma once

#include <glm/glm.hpp>

namespace Xaloc {

	class Scene;
	class Entity;
	
	class PhysicSpace
	{
	// Public API
	public:

		template<typename T>
		bool PlaceMeeting(const Entity& entity);

		template<typename T>
		bool PlaceMeeting(const glm::vec3& position, const Entity& entity);
		


	// Scene-shared API
	private:

		// TODO
		

		friend class Scene;
	};

	
}