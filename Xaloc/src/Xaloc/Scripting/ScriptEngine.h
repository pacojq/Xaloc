#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Core/Timestep.h"

#include "Xaloc/Scene/Entity.h"

#include "Xaloc/Scene/Components/BehaviourComponent.h"

#include <string>

extern "C" {
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClassField MonoClassField;
}

namespace Xaloc {


	struct EntityBehaviourClass;

	struct EntityInstance
	{
		EntityBehaviourClass* ScriptClass;

		uint32_t Handle = 0;
		Scene* SceneInstance;

		MonoObject* GetInstance();
	};



	enum class FieldType
	{
		None = 0, Float, Int, UnsignedInt, String, Vec2, Vec3, Vec4
	};

	struct PublicField
	{
		std::string Name;
		FieldType Type;

		PublicField(const std::string& name, FieldType type)
			: Name(name), Type(type) {}

		template<typename T>
		T GetValue() const
		{
			T value;
			GetValue_Internal(&value);
			return value;
		}

		template<typename T>
		void SetValue(T value) const
		{
			SetValue_Internal(&value);
		}
	private:
		EntityInstance* m_EntityInstance;
		MonoClassField* m_MonoClassField;

		void SetValue_Internal(void* value) const;
		void GetValue_Internal(void* outValue) const;

		friend class ScriptEngine;
	};

	

	
	using ScriptModuleFieldMap = std::unordered_map<std::string, std::unordered_map<std::string, PublicField>>;

	/// <summary>
	/// Public fields in an EntityInstance.
	/// </summary>
	struct EntityInstanceFields
	{
		EntityInstance Instance;
		ScriptModuleFieldMap ModuleFieldMap;
	};
	
	
	/// <summary>
	/// Two nested maps: the first, [UUID, map] represents a scene;
	/// the second, [UUID, EntityInstanceFields], represents the entity.
	/// </summary>
	using EntityInstanceMap = std::unordered_map<UUID, std::unordered_map<UUID, EntityInstanceFields>>;

	
	
	class ScriptEngine
	{
	public:
		static void Init(const std::string& assemblyPath);
		static void Shutdown();

		static bool ModuleExists(const std::string& moduleName);
		
		static void InitBehaviourEntity(Entity entity);
		static void ShutdownBehaviourEntity(Entity entity, const std::string& moduleName);

		static void InstantiateEntityClass(Entity entity);
		
		
		static void OnCreateEntity(Entity entity);
		static void OnCreateEntity(UUID sceneID, UUID entityID);
		
		static void OnUpdateEntity(UUID sceneID, UUID entityID, Timestep ts);

		static EntityInstanceMap& GetEntityInstanceMap();
		static EntityInstanceFields& GetEntityInstanceFields(UUID sceneID, UUID entityID);

		static void SetSceneContext(const Ref<Scene>& scene);
		static const Ref<Scene>& GetCurrentSceneContext();
	};

}