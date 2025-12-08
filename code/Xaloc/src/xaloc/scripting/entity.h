#pragma once
#include "xaloc/core/core.h"
#include "xaloc/core/guid.h"
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/object.h>
#include <string>

namespace Xaloc {
	class ZScene;
	struct EntityBehaviourClass;
}


namespace Scripting
{

/// <summary>
/// Reference to a behaviour class implementation in C#.
/// </summary>
class ZEntityBehaviourClass
{
public:
	std::string FullName;
	std::string ClassName;
	std::string NamespaceName;

	MonoClass* Class;
	MonoMethod* MethodOnCreate;
	MonoMethod* MethodOnDestroy;
	MonoMethod* MethodOnUpdate;

	void InitClassMethods(MonoImage* image);
};

struct SEntityInstance
{
	ZEntityBehaviourClass* ScriptClass;

	uint32_t Handle = 0;
	Xaloc::ZScene* SceneInstance;
};

enum class EFieldType
{
	None = 0, Float, Int, UnsignedInt, String, Vec2, Vec3, Vec4
};

class ZPublicField
{
public:
	std::string Name;
	EFieldType Type;

	ZPublicField(const std::string& sName, EFieldType sType);

	void Bind(SEntityInstance* pEntityInstance, MonoClassField* pMonoClassField);

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
	SEntityInstance* m_pEntityInstance;
	MonoClassField* m_pMonoClassField;

	void SetValue_Internal(void* value) const;
	void GetValue_Internal(void* outValue) const;
};

// TODO: this is messy as hell.
//   - make SEntityInstanceFields a class to interface this map
//   - rethink a bit the whole thing
using ZScriptModuleFieldMap = std::unordered_map<std::string, std::unordered_map<std::string, ZPublicField>>;

/// <summary>
/// Public fields in an EntityInstance.
/// </summary>
struct SEntityInstanceFields // TODO: make this a class. Make an interface to edit the fields map
{
	SEntityInstance Instance;
	ZScriptModuleFieldMap ModuleFieldMap;
};

/// <summary>
/// Two nested maps: the first, [UUID, map] represents a scene;
/// the second, [UUID, EntityInstanceFields], represents the entity.
/// </summary>
using ZEntityInstanceMap = std::unordered_map<Xaloc::ZGuid, std::unordered_map<Xaloc::ZGuid, SEntityInstanceFields>>;

EFieldType GetXalocFieldType(MonoType* pFieldType);

EFieldType GetXalocFieldType(MonoClassField* pField);

MonoObject* GetMonoObjectInstance(const SEntityInstance& instance);

}