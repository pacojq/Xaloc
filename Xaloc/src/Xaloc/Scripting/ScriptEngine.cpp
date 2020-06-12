#include "xapch.h"
#include "ScriptEngine.h"

#include "ScriptEngineRegistry.h"

#include "Xaloc/Scripting/Engine/MonoUtils.h"
#include "Xaloc/Scripting/Engine/Entity.h"

#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>

#include <iostream>

#include <Windows.h>

namespace Xaloc {

	static bool s_Initialized = false;
	
	static MonoDomain* s_MonoDomain = nullptr;
	static std::string s_AssemblyPath;

	static ScriptModuleFieldMap s_PublicFields;

	static std::unordered_map<std::string, EntityBehaviourClass> s_BehaviourClassMap;
	static std::unordered_map<uint32_t, EntityInstance> s_EntityInstanceMap;


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

		s_CoreAssembly = LoadAssembly("assets/scripts/XalocSharp.dll");
		s_CoreAssemblyImage = GetAssemblyImage(s_CoreAssembly);

		s_AppAssembly = LoadAssembly(s_AssemblyPath);
		s_AppAssemblyImage = GetAssemblyImage(s_AppAssembly);
		ScriptEngineRegistry::RegisterAll();
	}

	void ScriptEngine::Shutdown()
	{
		// TODO shutdown mono
	}


	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		EntityInstance& entityInstance = s_EntityInstanceMap[(uint32_t)entity.m_EntityHandle];
		if (entityInstance.ScriptClass->MethodOnCreate)
			MonoUtils::CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->MethodOnCreate);
	}

	void ScriptEngine::OnUpdateEntity(uint32_t entityID, Timestep ts)
	{
		if (!s_Initialized)
			return;
		
		XA_CORE_ASSERT(s_EntityInstanceMap.find(entityID) != s_EntityInstanceMap.end(), "Could not find entity in instance map!");

		auto& entity = s_EntityInstanceMap[entityID];

		if (entity.ScriptClass->MethodOnUpdate)
		{
			void* args[] = { &ts };
			MonoUtils::CallMethod(entity.GetInstance(), entity.ScriptClass->MethodOnUpdate, args);
		}
	}

	void ScriptEngine::OnInitEntity(BehaviourComponent& behaviour, uint32_t entityID, uint32_t sceneID)
	{
		if (!s_Initialized)
			return;

		EntityBehaviourClass& behaviourClass = s_BehaviourClassMap[behaviour.ModuleName];
		behaviourClass.FullName = behaviour.ModuleName;
		if (behaviour.ModuleName.find('.') != std::string::npos)
		{
			behaviourClass.NamespaceName = behaviour.ModuleName.substr(0, behaviour.ModuleName.find_last_of('.'));
			behaviourClass.ClassName = behaviour.ModuleName.substr(behaviour.ModuleName.find_last_of('.') + 1);
		}
		else
		{
			behaviourClass.ClassName = behaviour.ModuleName;
		}

		behaviourClass.Class = GetClass(s_AppAssemblyImage, behaviourClass);
		behaviourClass.InitClassMethods(s_AppAssemblyImage);

		EntityInstance& entityInstance = s_EntityInstanceMap[entityID];
		entityInstance.ScriptClass = &behaviourClass;
		entityInstance.Handle = Instantiate(behaviourClass);

		MonoProperty* entityIDPropery = mono_class_get_property_from_name(behaviourClass.Class, "EntityID");
		mono_property_get_get_method(entityIDPropery);
		MonoMethod* entityIDSetMethod = mono_property_get_set_method(entityIDPropery);
		void* param[] = { &entityID };
		MonoUtils::CallMethod(entityInstance.GetInstance(), entityIDSetMethod, param);

		MonoProperty* sceneIDPropery = mono_class_get_property_from_name(behaviourClass.Class, "SceneID");
		mono_property_get_get_method(sceneIDPropery);
		MonoMethod* sceneIDSetMethod = mono_property_get_set_method(sceneIDPropery);
		param[0] = { &sceneID };
		MonoUtils::CallMethod(entityInstance.GetInstance(), sceneIDSetMethod, param);

		if (behaviourClass.MethodOnCreate)
			MonoUtils::CallMethod(entityInstance.GetInstance(), behaviourClass.MethodOnCreate);

		// Retrieve public fields
		{
			MonoClassField* iter;
			void* ptr = 0;
			while ((iter = mono_class_get_fields(behaviourClass.Class, &ptr)) != NULL)
			{
				const char* name = mono_field_get_name(iter);
				uint32_t flags = mono_field_get_flags(iter);
				if (flags & MONO_FIELD_ATTR_PUBLIC == 0)
					continue;

				MonoType* fieldType = mono_field_get_type(iter);
				FieldType xalocFieldType = GetXalocFieldType(fieldType);

				// TODO: Attributes
				MonoCustomAttrInfo* attr = mono_custom_attrs_from_field(behaviourClass.Class, iter);

				auto& publicField = s_PublicFields[behaviour.ModuleName].emplace_back(name, xalocFieldType);
				publicField.m_EntityInstance = &entityInstance;
				publicField.m_MonoClassField = iter;
				// mono_field_set_value(entityInstance.Instance, iter, )
			}
		}
	}

	const Xaloc::ScriptModuleFieldMap& ScriptEngine::GetFieldMap()
	{
		return s_PublicFields;
	}
	
	
}