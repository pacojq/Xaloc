#pragma once

#include "Xaloc/Core/Timestep.h"

#include <glm/glm.hpp>


namespace Xaloc {


	class Component
	{
	public:

		virtual void OnUpdate(Timestep ts) = 0;

		glm::vec3 GetLocalPosition() const { return m_Position; }
		void SetLocalPosition(glm::vec3 position) { m_Position = position; }

	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	};

}