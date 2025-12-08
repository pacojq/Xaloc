#pragma once
#include "xaloc/core/core.h"
#include "xaloc/core/time.h"
#include "xaloc/scene/scene.h"
#include "xaloc/scene/entity.h"
#include "xaloc/scene/components/behaviour.h"
#include "xaloc/scripting/entity.h"
#include <string>


namespace Scripting
{

class ZScriptEngine
{
public:
	static void Init(const std::string& assemblyPath);
	static void Shutdown();

	static bool ModuleExists(const std::string& moduleName);

	static void InitBehaviourEntity(Xaloc::ZEntity entity);
	static void ShutdownBehaviourEntity(Xaloc::ZEntity entity, const std::string& moduleName);

	static void InstantiateEntityClass(Xaloc::ZEntity entity);

	static void OnCreateEntity(Xaloc::ZEntity entity);
	static void OnCreateEntity(Xaloc::ZGuid sceneID, Xaloc::ZGuid entityID);

	static void OnUpdateEntity(Xaloc::ZGuid sceneID, Xaloc::ZGuid entityID, Xaloc::ZTimestep ts);

	static ZEntityInstanceMap& GetEntityInstanceMap();
	static SEntityInstanceFields& GetEntityInstanceFields(Xaloc::ZGuid sceneID, Xaloc::ZGuid entityID);

	static void SetSceneContext(const Xaloc::ZRef<Xaloc::ZScene>& scene);
	static const Xaloc::ZRef<Xaloc::ZScene>& GetCurrentSceneContext();
};

}