#pragma once
#include "xaloc/core/guid.h"
#include "xaloc/core/time.h"
#include "xaloc/Events/Event.h"
#include "xaloc/scene/camerastack.h"
#include "xaloc/scene/collision.h"
#include <entt/entt.hpp>

namespace Xaloc {
	class ZEditorCamera;
	class ZEntity;
	class ZSceneSerializer;
}


namespace Xaloc
{

using EntityMap = std::unordered_map<ZGuid, ZEntity>;
	
class ZScene
{
public:

	static void Save(
		const ZRef<ZScene>& scene,
		const std::string& filename);
	
	static ZRef<ZScene> Load(
		const std::string& filename);

	ZScene(const std::string& sName = "Scene");
	ZScene(const std::string& sName, ZGuid id);
	~ZScene();

	void Init();

	void StartRuntime();

	void OnUpdateRuntime(ZTimestep ts);
	void OnUpdateEditor(ZTimestep ts, glm::mat4 cameraViewProj);

	void OnEvent(ZEvent& rEvent);

	const ZRef<ZCameraStack>& GetCameraStack() const { return m_pCameraStack; }

	ZEntity ZScene::CreateEntity(const std::string& sName);
	ZEntity ZScene::CreateEntity(const std::string& sName, ZGuid id);
	void DestroyEntity(ZEntity entity);

	template<typename T>
	auto FindEntitiesWith()
	{
		return m_Registry.view<T>();
	}

	inline ZGuid const GetID() const { return m_SceneID; }
	inline const EntityMap& GetEntityMap() const { return m_EntityMap; }

	// TODO: find a better way of exposing entities
	entt::registry& WriteRegistry()
	{
		return m_Registry;
	}

	inline void SetName(const char* sName)
	{
		m_Name = sName;
	}

private:
	ZGuid m_SceneID;
	std::string m_Name;
	entt::entity m_SceneEntity;

	ZRef<ZCameraStack> m_pCameraStack;

	entt::registry m_Registry;
	EntityMap m_EntityMap;

	ZRef<ZPhysicSpace> m_PhysicSpace;

	// TODO: get rid of friend classes

	friend class ZEntity;
	friend class ZSceneSerializer;

	friend void OnBehaviourComponentConstruct(entt::registry& registry, entt::entity entity);
};

}
