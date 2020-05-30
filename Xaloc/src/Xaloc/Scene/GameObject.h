#pragma once

#include "Xaloc/Core/Timestep.h"

#include <glm\glm.hpp>
#include <vector>

namespace Xaloc {

	class Component;

	struct Transform
	{
		glm::vec3 Position;
		glm::vec3 Scale;
		float Rotation;
	};



	class GameObject
	{
	public:
		~GameObject();

		void OnUpdate(Timestep ts);


		void AddComponent(Component* component);

		void SetPosition(glm::vec3 position) { m_Position = position; }
		glm::vec3 GetPosition() const { return m_Position; }
		
	private:
		GameObject(const std::string& name);



	private:
		std::string m_Name;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		//Transform& m_Transform;
		std::vector<Component*> m_Components;

		friend class Scene;
	};

}