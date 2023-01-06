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
		T& GetComponent() const
		{
			XA_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() const { return m_Scene->m_Registry.has<T>(m_EntityHandle); }


		template<typename T>
		void RemoveComponent()
		{
			XA_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		Scene* const GetScene() const { return m_Scene; }
		

		void SetTransform(glm::mat4* trans)
		{ 
			auto& transformComponent = GetComponent<TransformComponent>();
			
			glm::vec3 scale, translation, skew;
			glm::vec4 perspective;
			glm::quat orientation;
			glm::decompose(*trans, scale, orientation, translation, skew, perspective);

			if (glm::abs(orientation.y < 0.001f))
				orientation.y = 0.0f;

			transformComponent.Translation = translation;
			transformComponent.Rotation = glm::degrees(glm::eulerAngles(orientation));
			transformComponent.Scale = scale;
		}

		

	public:
		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t () const { return (uint32_t)m_EntityHandle; }
		
		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
		

	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;

		std::string m_Name;

		friend class Scene;
		friend class ScriptEngine;
	};

}