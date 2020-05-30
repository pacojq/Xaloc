#pragma once

#include "GameObject.h"
#include "Xaloc/Core/Timestep.h"

#include <glm/glm.hpp>


namespace Xaloc {


	class Component
	{
	public:

		virtual void OnUpdate(Timestep ts) = 0;


		GameObject* GetGameObject() const { return m_GameObject; }


		glm::vec3 GetPosition() const
		{
			return m_GameObject == nullptr ? m_Position :
				m_GameObject->GetPosition() + m_Position;
		}

		glm::vec3 GetLocalPosition() const { return m_Position; }
		void SetLocalPosition(glm::vec3 position) { m_Position = position; }


	private:
		void Bind(GameObject* go);

	private:
		GameObject* m_GameObject;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		friend class GameObject;
	};

}