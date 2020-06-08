#pragma once

#include "Xaloc/Scripting/Engine/Field.h"

#include "Xaloc/Core/Core.h"
#include "Xaloc/Core/Timestep.h"

#include "Xaloc/Scene/Components.h"
#include "Xaloc/Scene/Entity.h"

#include <string>
#include <mono/metadata/assembly.h>

extern "C" {
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClassField MonoClassField;
}

namespace Xaloc {

	using ScriptModuleFieldMap = std::unordered_map<std::string, std::vector<PublicField>>;

	class ScriptEngine
	{
	public:
		static void Init(const std::string& assemblyPath);
		static void Shutdown();

		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(uint32_t entityID, Timestep ts);

		static void OnInitEntity(BehaviourComponent& behaviour, uint32_t entityID, uint32_t sceneID);

		static const ScriptModuleFieldMap& GetFieldMap();

	};

}