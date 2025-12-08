#include "xapch.h"
#include "xaloc/scripting/engine.h"
#include "xaloc/scripting/registry.h"
#include "xaloc/debugging/assert.h"
#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/assembly.h>
#include <iostream>
#include <Windows.h>


namespace Scripting
{

static MonoAssembly* s_pAppAssembly = nullptr;
static MonoAssembly* s_pCoreAssembly = nullptr;
static MonoImage* s_pAppAssemblyImage = nullptr;
static MonoImage* s_pCoreAssemblyImage = nullptr;

static bool s_bInitialized = false;
	
static MonoDomain* s_pMonoDomain = nullptr;
static std::string s_sAssemblyPath;

static Xaloc::ZRef<Xaloc::ZScene> s_SceneContext;
	
static std::unordered_map<std::string, ZEntityBehaviourClass> s_BehaviourClassMap;
static ZEntityInstanceMap s_EntityInstanceMap;

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

static MonoClass* GetClass(MonoImage* image, const ZEntityBehaviourClass& behaviourClass)
{
	MonoClass* monoClass = mono_class_from_name(image, behaviourClass.NamespaceName.c_str(), behaviourClass.ClassName.c_str());
	if (!monoClass)
		std::cout << "mono_class_from_name failed" << std::endl;

	return monoClass;
}

static uint32_t Instantiate(const ZEntityBehaviourClass& behaviourClass)
{
	MonoObject* instance = mono_object_new(s_pMonoDomain, behaviourClass.Class);
	if (!instance)
		std::cout << "mono_object_new failed" << std::endl;

	mono_runtime_object_init(instance);
	uint32_t handle = mono_gchandle_new(instance, false);
	return handle;
}

static MonoObject* CallMethod(MonoObject* pInstance, MonoMethod* pMethod, void** aParams = nullptr)
{
	MonoObject* pException = NULL;
	MonoObject* pResult = mono_runtime_invoke(pMethod, pInstance, aParams, &pException);
	// TODO: exception handling
	return pResult;
}

static MonoObject* CallMethod(const SEntityInstance& instance, MonoMethod* pMethod, void** aParams = nullptr)
{
	MonoObject* pInstance = GetMonoObjectInstance(instance);
	return CallMethod(pInstance, pMethod, aParams);
}

void ZScriptEngine::Init(const std::string& assemblyPath)
{
	s_bInitialized = true;
	s_sAssemblyPath = assemblyPath;

	// INIT MONO
	// TODO better distribute the mono/lib folder
	mono_set_assemblies_path("mono/lib");
	mono_jit_set_trace_options("--verbose");

	auto domain = mono_jit_init("Xaloc");

	char* name = (char*)"XalocRuntime";
	s_pMonoDomain = mono_domain_create_appdomain(name, nullptr);

	// LOAD XALOC RUNTIME ASSEMBLY
	if (s_pAppAssembly)
	{
		mono_domain_unload(s_pMonoDomain);
		mono_assembly_close(s_pAppAssembly);
		char* name = (char*)"XalocRuntime";
		s_pMonoDomain = mono_domain_create_appdomain(name, nullptr);
	}

	s_pCoreAssembly = LoadAssembly("assets/game/scripts/XalocSharp.dll"); // TODO: parametrize this better! - use engine ini files
	s_pCoreAssemblyImage = GetAssemblyImage(s_pCoreAssembly);
	s_pAppAssembly = LoadAssembly(s_sAssemblyPath);
	s_pAppAssemblyImage = GetAssemblyImage(s_pAppAssembly);

	RegisterEngineMethods(s_pCoreAssemblyImage);
}

void ZScriptEngine::Shutdown()
{
	// TODO shutdown mono
}

bool ZScriptEngine::ModuleExists(const std::string& moduleName)
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
	MonoClass* monoClass = mono_class_from_name(s_pAppAssemblyImage, NamespaceName.c_str(), ClassName.c_str());
	return monoClass != nullptr;
}

void ZScriptEngine::InitBehaviourEntity(Xaloc::ZEntity entity)
{
	if (!s_bInitialized)
	{
		XA_CORE_ERROR("Cannot init Behaviour when the ZScriptEngine is not initialized!");
		return;
	}

	Xaloc::ZScene* pScene = entity.GetScene();
	Xaloc::ZGuid id = entity.GetComponent<Xaloc::CmpId>().idValue;
	const std::string& sModuleName = entity.GetComponent<Xaloc::CmpBehaviour>().sModuleName;
	if (sModuleName.empty())
	{
		return;
	}
	XA_CORE_INFO("ZScriptEngine::InitBehaviourEntity -> Id = {}", id.Value());
		
	if (!ModuleExists(sModuleName))
	{
		XA_CORE_ERROR("Entity references non-existent script module '{0}'", sModuleName);
		return;
	}

	ZEntityBehaviourClass& scriptClass = s_BehaviourClassMap[sModuleName];
	scriptClass.FullName = sModuleName;
	if (sModuleName.find('.') != std::string::npos)
	{
		scriptClass.NamespaceName = sModuleName.substr(0, sModuleName.find_last_of('.'));
		scriptClass.ClassName = sModuleName.substr(sModuleName.find_last_of('.') + 1);
	}
	else
	{
		scriptClass.ClassName = sModuleName;
	}

	scriptClass.Class = GetClass(s_pAppAssemblyImage, scriptClass);
	scriptClass.InitClassMethods(s_pAppAssemblyImage);

	SEntityInstanceFields& entityInstanceData = s_EntityInstanceMap[pScene->GetID()][id];
	SEntityInstance& entityInstance = entityInstanceData.Instance;
	entityInstance.ScriptClass = &scriptClass;
	ZScriptModuleFieldMap& moduleFieldMap = entityInstanceData.ModuleFieldMap;
	auto& fieldMap = moduleFieldMap[sModuleName];

	// TODO: fix this mess

	// Save old fields
	std::unordered_map<std::string, ZPublicField> oldFields;
	oldFields.reserve(fieldMap.size());
	for (auto& [fieldName, field] : fieldMap)
	{
		oldFields.emplace(fieldName, std::move(field));
	}
	fieldMap.clear();

	// Retrieve public fields (TODO: cache these fields if the module is used more than once)
	{
		MonoClassField* pField;
		void* ptr = 0;
		while ((pField = mono_class_get_fields(scriptClass.Class, &ptr)) != NULL)
		{
			const char* sName = mono_field_get_name(pField);
			uint32_t flags = mono_field_get_flags(pField);
			if ((flags & MONO_FIELD_ATTR_PUBLIC) == 0)
			{
				continue;
			}
			EFieldType eFieldType = GetXalocFieldType(pField);

			// TODO: Attributes
			MonoCustomAttrInfo* attr = mono_custom_attrs_from_field(scriptClass.Class, pField);

			if (oldFields.find(sName) != oldFields.end())
			{
				fieldMap.emplace(sName, std::move(oldFields.at(sName)));
			}
			else
			{
				ZPublicField field = ZPublicField(sName, eFieldType);
				field.Bind(&entityInstance, pField);
				fieldMap.emplace(sName, std::move(field));
			}
		}
	}
}

void ZScriptEngine::ShutdownBehaviourEntity(Xaloc::ZEntity entity, const std::string& moduleName)
{
	Xaloc::ZGuid id = entity.GetComponent<Xaloc::CmpId>().idValue;
	SEntityInstanceFields& rEntityInstanceFields = GetEntityInstanceFields(entity.GetScene()->GetID(), id);
	ZScriptModuleFieldMap& rModuleFieldMap = rEntityInstanceFields.ModuleFieldMap;
	if (rModuleFieldMap.find(moduleName) != rModuleFieldMap.end())
	{
		rModuleFieldMap.erase(moduleName);
	}
}

void ZScriptEngine::InstantiateEntityClass(Xaloc::ZEntity entity)
{
	Xaloc::ZScene* pScene = entity.GetScene();
	Xaloc::ZGuid id = entity.GetComponent<Xaloc::CmpId>().idValue;
	auto& moduleName = entity.GetComponent<Xaloc::CmpBehaviour>().sModuleName;

	SEntityInstanceFields& entityInstanceData = GetEntityInstanceFields(pScene->GetID(), id);
	SEntityInstance& entityInstance = entityInstanceData.Instance;
	XA_CORE_ASSERT(entityInstance.ScriptClass, "No behaviour found!");
	entityInstance.Handle = Instantiate(*entityInstance.ScriptClass);

	XA_CORE_TRACE("Entity Behaviour instanciated with handle {0}", entityInstance.Handle);
		
	MonoProperty* entityIDPropery = mono_class_get_property_from_name(entityInstance.ScriptClass->Class, "ID");
	mono_property_get_get_method(entityIDPropery);
	MonoMethod* entityIDSetMethod = mono_property_get_set_method(entityIDPropery);
	void* param[] = { &id };
	CallMethod(entityInstance, entityIDSetMethod, param);

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

void ZScriptEngine::OnCreateEntity(Xaloc::ZEntity entity)
{
	OnCreateEntity(entity.GetScene()->GetID(), entity.GetComponent<Xaloc::CmpId>().idValue);
}

void ZScriptEngine::OnCreateEntity(Xaloc::ZGuid sceneID, Xaloc::ZGuid entityID)
{
	if (!s_bInitialized)
	{
		return;
	}
	const SEntityInstance& entityInstance = GetEntityInstanceFields(sceneID, entityID).Instance;
	if (entityInstance.ScriptClass->MethodOnCreate)
	{
		CallMethod(entityInstance, entityInstance.ScriptClass->MethodOnCreate);
	}
}

void ZScriptEngine::OnUpdateEntity(Xaloc::ZGuid sceneID, Xaloc::ZGuid entityID, Xaloc::ZTimestep ts)
{
	if (!s_bInitialized)
	{
		return;
	}
	const SEntityInstance& entityInstance = GetEntityInstanceFields(sceneID, entityID).Instance;
	if (entityInstance.ScriptClass->MethodOnUpdate)
	{
		void* args[] = { &ts };
		CallMethod(entityInstance, entityInstance.ScriptClass->MethodOnUpdate, args);
	}
}

ZEntityInstanceMap& ZScriptEngine::GetEntityInstanceMap()
{
	return s_EntityInstanceMap;
}

SEntityInstanceFields& ZScriptEngine::GetEntityInstanceFields(Xaloc::ZGuid sceneID, Xaloc::ZGuid entityID)
{
	XA_CORE_ASSERT(s_EntityInstanceMap.find(sceneID) != s_EntityInstanceMap.end(), "Invalid scene ID!");
	auto& entityIDMap = s_EntityInstanceMap.at(sceneID);
	XA_CORE_ASSERT(entityIDMap.find(entityID) != entityIDMap.end(), "Invalid entity ID!");
	return entityIDMap.at(entityID);
}

void ZScriptEngine::SetSceneContext(const Xaloc::ZRef<Xaloc::ZScene>& scene)
{
	s_SceneContext = scene;
}

const Xaloc::ZRef<Xaloc::ZScene>& ZScriptEngine::GetCurrentSceneContext()
{
	return s_SceneContext;
}

}