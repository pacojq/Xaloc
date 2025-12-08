#pragma once
#include "xaloc/core/time.h"
#include "xaloc/debugging/assert.h"
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Xaloc {
	class ZScene;
}


namespace Xaloc
{

class ZEntity
{
public:
	ZEntity() = default;
	ZEntity(entt::entity handle, ZScene* scene)
		: m_EntityHandle(handle), m_Scene(scene) {}
		
	~ZEntity() {}
		
	template<typename T, typename... Args>
	T& AddComponent(Args&&... args) { return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...); }

	template<typename T>
	T& GetComponent() const
	{
		XA_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
		return m_Scene->m_Registry.get<T>(m_EntityHandle);
	}

	template<typename T>
	bool HasComponent() const { return m_Scene->m_Registry.any_of<T>(m_EntityHandle); }

	template<typename T>
	void RemoveComponent()
	{
		XA_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
		m_Scene->m_Registry.remove<T>(m_EntityHandle);
	}

	ZScene* const GetScene() const { return m_Scene; }

#if DEPRECATED
	void SetTransform(glm::mat4 trans);
#endif

public:
	operator bool() const { return m_EntityHandle != entt::null; }
	operator entt::entity() const { return m_EntityHandle; }
	operator uint32_t () const { return (uint32_t)m_EntityHandle; }
		
	bool operator==(const ZEntity& other) const
	{
		return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
	}

	bool operator!=(const ZEntity& other) const
	{
		return !(*this == other);
	}
		

private:
	entt::entity m_EntityHandle = entt::null;
	ZScene* m_Scene = nullptr;

	std::string m_Name;

	friend class ZScene;
	friend class ScriptEngine;
};

}