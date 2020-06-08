#pragma once

#include "Scene.h"
#include "Xaloc/Core/Timestep.h"

#include "Xaloc/Scene/Components.h"

#include <entt/entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Xaloc {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene)
			: m_EntityHandle(handle), m_Scene(scene) {}
		
		~Entity() {}
		

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) { return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...); }

		template<typename T>
		T& GetComponent() { return m_Scene->m_Registry.get<T>(m_EntityHandle); }

		template<typename T>
		bool HasComponent() { return m_Scene->m_Registry.has<T>(m_EntityHandle); }


		inline glm::mat4& Transform() { return m_Scene->m_Registry.get<TransformComponent>(m_EntityHandle); }
		inline const glm::mat4& Transform() const { return m_Scene->m_Registry.get<TransformComponent>(m_EntityHandle); }

		void SetTransform(glm::mat4* trans)
		{ 
			auto& transformComponent = GetComponent<TransformComponent>();
			memcpy(glm::value_ptr(transformComponent.Transform), trans, sizeof(glm::mat4));
		}

		operator uint32_t () const { return (uint32_t)m_EntityHandle; }
		

	private:
		entt::entity m_EntityHandle;
		Scene* m_Scene = nullptr;

		std::string m_Name;

		friend class Scene;
		friend class ScriptEngine;
	};

}