#include "xapch.h"
#include "ScriptEngine.h"

#include "ScriptEngineRegistry.h"

#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/assembly.h>

#include <iostream>

#include <Windows.h>

namespace Xaloc {



	static MonoMethod* GetMethod(MonoImage* image, const std::string& methodDesc)
	{
		MonoMethodDesc* desc = mono_method_desc_new(methodDesc.c_str(), NULL);
		if (!desc)
			std::cout << "mono_method_desc_new failed" << std::endl;

		MonoMethod* method = mono_method_desc_search_in_image(desc, image);
		if (!method)
			std::cout << "mono_method_desc_search_in_image failed" << std::endl;

		return method;
	}

	struct EntityBehaviourClass
	{
		std::string FullName;
		std::string ClassName;
		std::string NamespaceName;

		MonoClass* Class;
		MonoMethod* MethodOnCreate;
		MonoMethod* MethodOnDestroy;
		MonoMethod* MethodOnUpdate;

		void InitClassMethods(MonoImage* image)
		{
			MethodOnCreate = GetMethod(image, FullName + ":OnCreate()");
			MethodOnUpdate = GetMethod(image, FullName + ":OnUpdate(single)");
		}
	};

	MonoObject* EntityInstance::GetInstance()
	{
		return mono_gchandle_get_target(Handle);
	}










	static FieldType GetXalocFieldType(MonoType* monoType)
	{
		int type = mono_type_get_type(monoType);
		switch (type)
		{
		case MONO_TYPE_R4: return FieldType::Float;
		case MONO_TYPE_I4: return FieldType::Int;
		case MONO_TYPE_U4: return FieldType::UnsignedInt;
		case MONO_TYPE_STRING: return FieldType::String;
		case MONO_TYPE_VALUETYPE:
		{
			char* name = mono_type_get_name(monoType);
			if (strcmp(name, "Xaloc.Vector2") == 0) return FieldType::Vec2;
			if (strcmp(name, "Xaloc.Vector3") == 0) return FieldType::Vec3;
			if (strcmp(name, "Xaloc.Vector4") == 0) return FieldType::Vec4;
		}
		}
		return FieldType::None;
	}

	void PublicField::SetValue_Internal(void* value) const
	{
		mono_field_set_value(m_EntityInstance->GetInstance(), m_MonoClassField, value);
	}

	void PublicField::GetValue_Internal(void* outValue) const
	{
		mono_field_get_value(m_EntityInstance->GetInstance(), m_MonoClassField, outValue);
	}





	




	
	static bool s_Initialized = false;
	
	static MonoDomain* s_MonoDomain = nullptr;
	static std::string s_AssemblyPath;

	static Ref<Scene> s_SceneContext;
	
	static std::unordered_map<std::string, EntityBehaviourClass> s_BehaviourClassMap;
	static EntityInstanceMap s_EntityInstanceMap;


	MonoAssembly* LoadAssemblyFromFile(const char* filepath)
	{
		if (filepath == NULL)
		{
			return NULL;
		}

		HANDLE file = CreateFileA(filepath, FILE_READ_ACCESS, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			return NULL;
		}

		DWORD file_size = GetFileSize(file, NULL);
		if (file_size == INVALID_FILE_SIZE)
		{
			CloseHandle(file);
			return NULL;
		}

		void* file_data = malloc(file_size);
		if (file_data == NULL)
		{
			CloseHandle(file);
			return NULL;
		}

		DWORD read = 0;
		ReadFile(file, file_data, file_size, &read, NULL);
		if (file_size != read)
		{
			free(file_data);
			CloseHandle(file);
			return NULL;
		}

		MonoImageOpenStatus status;
		//MonoImage* image = mono_image_open_from_data_with_name(reinterpret_cast<char*>(file_data), file_size, 1, &status, 0, full_file_path);
		MonoImage* image = mono_image_open_from_data_full(reinterpret_cast<char*>(file_data), file_size, 1, &status, 0);
		if (status != MONO_IMAGE_OK)
		{
			return NULL;
		}
		auto assemb = mono_assembly_load_from_full(image, filepath, &status, 0);
		free(file_data);
		CloseHandle(file);
		mono_image_close(image);
		return assemb;
	}


	static MonoAssembly* LoadAssembly(const std::string& path)
	{
		MonoAssembly* assembly = LoadAssemblyFromFile(path.c_str()); //mono_domain_assembly_open(s_MonoDomain, path.c_str());

		if (!assembly)
			std::cout << "Could not load assembly: " << path << std::endl;
		else
			std::cout << "Successfully loaded assembly: " << path << std::endl;

		return assembly;
	}

	static MonoImage* GetAssemblyImage(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		if (!image)
			std::cout << "mono_assembly_get_image failed" << std::endl;

		return image;
	}

	static MonoClass* GetClass(MonoImage* image, const EntityBehaviourClass& behaviourClass)
	{
		MonoClass* monoClass = mono_class_from_name(image, behaviourClass.NamespaceName.c_str(), behaviourClass.ClassName.c_str());
		if (!monoClass)
			std::cout << "mono_class_from_name failed" << std::endl;

		return monoClass;
	}

	static uint32_t Instantiate(EntityBehaviourClass& behaviourClass)
	{
		MonoObject* instance = mono_object_new(s_MonoDomain, behaviourClass.Class);
		if (!instance)
			std::cout << "mono_object_new failed" << std::endl;

		mono_runtime_object_init(instance);
		uint32_t handle = mono_gchandle_new(instance, false);
		return handle;
	}




	static MonoObject* CallMethod(MonoObject* object, MonoMethod* method, void** params = nullptr)
	{
		MonoObject* pException = NULL;
		MonoObject* result = mono_runtime_invoke(method, object, params, &pException);
		return result;
	}

	


	
	static MonoAssembly* s_AppAssembly = nullptr;
	static MonoAssembly* s_CoreAssembly = nullptr;
	MonoImage* s_AppAssemblyImage = nullptr;
	MonoImage* s_CoreAssemblyImage = nullptr;

	

	void ScriptEngine::Init(const std::string& assemblyPath)
	{
		s_Initialized = true;
		s_AssemblyPath = assemblyPath;

		
		// ============================ INIT MONO ============================//

		// TODO better distribute the mono/lib folder
		mono_set_assemblies_path("mono/lib");
		mono_jit_set_trace_options("--verbose");
		
		auto domain = mono_jit_init("Xaloc");

		char* name = (char*)"XalocRuntime";
		s_MonoDomain = mono_domain_create_appdomain(name, nullptr);



		// =================== LOAD XALOC RUNTIME ASSEMBLY ===================//
		
		if (s_AppAssembly)
		{
			mono_domain_unload(s_MonoDomain);
			mono_assembly_close(s_AppAssembly);

			char* name = (char*)"XalocRuntime";
			s_MonoDomain = mono_domain_create_appdomain(name, nullptr);

		}

		s_CoreAssembly = LoadAssembly("assets/game/scripts/XalocSharp.dll");
		s_CoreAssemblyImage = GetAssemblyImage(s_CoreAssembly);

		s_AppAssembly = LoadAssembly(s_AssemblyPath);
		s_AppAssemblyImage = GetAssemblyImage(s_AppAssembly);
		ScriptEngineRegistry::RegisterAll();
	}

	void ScriptEngine::Shutdown()
	{
		// TODO shutdown mono
	}





	bool ScriptEngine::ModuleExists(const std::string& moduleName)
	{
		std::string NamespaceName, ClassName;
		if (moduleName.find('.') != std::string::npos)
		{
			NamespaceName = moduleName.substr(0, moduleName.find_last_of('.'));
			ClassName = moduleName.substr(moduleName.find_last_of('.') + 1);
		}
		else
		{
			ClassName = moduleName;
		}

		MonoClass* monoClass = mono_class_from_name(s_AppAssemblyImage, NamespaceName.c_str(), ClassName.c_str());
		return monoClass != nullptr;
	}

	


	void ScriptEngine::InitBehaviourEntity(Entity entity)
	{
		if (!s_Initialized)
		{
			XA_CORE_ERROR("Cannot init Behaviour when the ScriptEngine is not initialized!");
			return;
		}

		Scene* scene = entity.m_Scene;
		UUID id = entity.GetComponent<IdComponent>().ID;
		auto& moduleName = entity.GetComponent<BehaviourComponent>().ModuleName;
		if (moduleName.empty())
			return;

		XA_CORE_INFO("ScriptEngine::InitBehaviourEntity -> Id = {}", id);
		
		if (!ModuleExists(moduleName))
		{
			XA_CORE_ERROR("Entity references non-existent script module '{0}'", moduleName);
			return;
		}

		EntityBehaviourClass& scriptClass = s_BehaviourClassMap[moduleName];
		scriptClass.FullName = moduleName;
		if (moduleName.find('.') != std::string::npos)
		{
			scriptClass.NamespaceName = moduleName.substr(0, moduleName.find_last_of('.'));
			scriptClass.ClassName = moduleName.substr(moduleName.find_last_of('.') + 1);
		}
		else
		{
			scriptClass.ClassName = moduleName;
		}

		scriptClass.Class = GetClass(s_AppAssemblyImage, scriptClass);
		scriptClass.InitClassMethods(s_AppAssemblyImage);

		EntityInstanceFields& entityInstanceData = s_EntityInstanceMap[scene->GetID()][id];
		EntityInstance& entityInstance = entityInstanceData.Instance;
		entityInstance.ScriptClass = &scriptClass;
		ScriptModuleFieldMap& moduleFieldMap = entityInstanceData.ModuleFieldMap;
		auto& fieldMap = moduleFieldMap[moduleName];

		// Save old fields
		std::unordered_map<std::string, PublicField> oldFields;
		oldFields.reserve(fieldMap.size());
		for (auto& [fieldName, field] : fieldMap)
			oldFields.emplace(fieldName, std::move(field));
		fieldMap.clear();

		// Retrieve public fields (TODO: cache these fields if the module is used more than once)
		{
			MonoClassField* iter;
			void* ptr = 0;
			while ((iter = mono_class_get_fields(scriptClass.Class, &ptr)) != NULL)
			{
				const char* name = mono_field_get_name(iter);
				uint32_t flags = mono_field_get_flags(iter);
				if ((flags & MONO_FIELD_ATTR_PUBLIC) == 0)
					continue;

				MonoType* fieldType = mono_field_get_type(iter);
				FieldType hazelFieldType = GetXalocFieldType(fieldType);

				// TODO: Attributes
				MonoCustomAttrInfo* attr = mono_custom_attrs_from_field(scriptClass.Class, iter);

				if (oldFields.find(name) != oldFields.end())
				{
					fieldMap.emplace(name, std::move(oldFields.at(name)));
				}
				else
				{
					PublicField field = { name, hazelFieldType };
					field.m_EntityInstance = &entityInstance;
					field.m_MonoClassField = iter;
					fieldMap.emplace(name, std::move(field));
				}
			}
		}
	}

	void ScriptEngine::ShutdownBehaviourEntity(Entity entity, const std::string& moduleName)
	{
		UUID id = entity.GetComponent<IdComponent>().ID;
		EntityInstanceFields& entityInstanceFields = GetEntityInstanceFields(entity.GetScene()->GetID(), id);
		ScriptModuleFieldMap& moduleFieldMap = entityInstanceFields.ModuleFieldMap;
		if (moduleFieldMap.find(moduleName) != moduleFieldMap.end())
			moduleFieldMap.erase(moduleName);
	}

	void ScriptEngine::InstantiateEntityClass(Entity entity)
	{
		Scene* scene = entity.m_Scene;
		UUID id = entity.GetComponent<IdComponent>().ID;
		auto& moduleName = entity.GetComponent<BehaviourComponent>().ModuleName;

		EntityInstanceFields& entityInstanceData = GetEntityInstanceFields(scene->GetID(), id);
		EntityInstance& entityInstance = entityInstanceData.Instance;
		XA_CORE_ASSERT(entityInstance.ScriptClass, "No behaviour found!");
		entityInstance.Handle = Instantiate(*entityInstance.ScriptClass);

		XA_CORE_TRACE("Entity Behaviour instanciated with handle {0}", entityInstance.Handle);
		
		MonoProperty* entityIDPropery = mono_class_get_property_from_name(entityInstance.ScriptClass->Class, "ID");
		mono_property_get_get_method(entityIDPropery);
		MonoMethod* entityIDSetMethod = mono_property_get_set_method(entityIDPropery);
		void* param[] = { &id };
		CallMethod(entityInstance.GetInstance(), entityIDSetMethod, param);

		// TODO Set all public fields to appropriate values
		//ScriptModuleFieldMap& moduleFieldMap = entityInstanceData.ModuleFieldMap;
		//if (moduleFieldMap.find(moduleName) != moduleFieldMap.end())
		//{
		//	auto& publicFields = moduleFieldMap.at(moduleName);
		//	for (auto& [name, field] : publicFields)
		//		field.CopyStoredValueToRuntime();
		//}

		// Call OnCreate function (if exists)
		OnCreateEntity(entity);
	}



	


	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		OnCreateEntity(entity.m_Scene->GetID(), entity.GetComponent<IdComponent>().ID);
	}

	void ScriptEngine::OnCreateEntity(UUID sceneID, UUID entityID)
	{
		if (!s_Initialized)
			return;
		
		EntityInstance& entityInstance = GetEntityInstanceFields(sceneID, entityID).Instance;
		if (entityInstance.ScriptClass->MethodOnCreate)
		{
			CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->MethodOnCreate);
		}
	}

	void ScriptEngine::OnUpdateEntity(UUID sceneID, UUID entityID, Timestep ts)
	{
		if (!s_Initialized)
			return;
		
		EntityInstance& entityInstance = GetEntityInstanceFields(sceneID, entityID).Instance;
		if (entityInstance.ScriptClass->MethodOnUpdate)
		{
			void* args[] = { &ts };
			CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->MethodOnUpdate, args);
		}
	}


	
	
	EntityInstanceMap& ScriptEngine::GetEntityInstanceMap()
	{
		return s_EntityInstanceMap;
	}

	EntityInstanceFields& ScriptEngine::GetEntityInstanceFields(UUID sceneID, UUID entityID)
	{
		XA_CORE_ASSERT(s_EntityInstanceMap.find(sceneID) != s_EntityInstanceMap.end(), "Invalid scene ID!");
		auto& entityIDMap = s_EntityInstanceMap.at(sceneID);
		XA_CORE_ASSERT(entityIDMap.find(entityID) != entityIDMap.end(), "Invalid entity ID!");
		return entityIDMap.at(entityID);
	}

	void ScriptEngine::SetSceneContext(const Ref<Scene>& scene)
	{
		s_SceneContext = scene;
	}

	const Ref<Scene>& ScriptEngine::GetCurrentSceneContext()
	{
		return s_SceneContext;
	}

}