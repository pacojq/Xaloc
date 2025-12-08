#include "xapch.h"
#include "xaloc/scripting/entity.h"
#include "xaloc/scene/scene.h"
#include <mono/jit/jit.h>
#include <iostream>
#include <Windows.h>


namespace Scripting
{

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

void ZEntityBehaviourClass::InitClassMethods(MonoImage* image)
{
	MethodOnCreate = GetMethod(image, FullName + ":OnCreate()");
	MethodOnUpdate = GetMethod(image, FullName + ":OnUpdate(single)");
}

ZPublicField::ZPublicField(const std::string& sName, EFieldType eType)
	: Name(sName)
	, Type(eType)
	, m_pEntityInstance(nullptr)
	, m_pMonoClassField(nullptr)
{
}


void ZPublicField::Bind(SEntityInstance* pEntityInstance, MonoClassField* pMonoClassField)
{
	m_pEntityInstance = pEntityInstance;
	m_pMonoClassField = pMonoClassField;
}

void ZPublicField::SetValue_Internal(void* value) const
{
	MonoObject* pMonoInstance = GetMonoObjectInstance(*m_pEntityInstance);
	mono_field_set_value(pMonoInstance, m_pMonoClassField, value);
}

void ZPublicField::GetValue_Internal(void* outValue) const
{
	MonoObject* pMonoInstance = GetMonoObjectInstance(*m_pEntityInstance);
	mono_field_get_value(pMonoInstance, m_pMonoClassField, outValue);
}

EFieldType GetXalocFieldType(MonoClassField* pField)
{
	MonoType* pFieldType = mono_field_get_type(pField);
	return GetXalocFieldType(pFieldType);
}

EFieldType GetXalocFieldType(MonoType* pFieldType)
{
	int type = mono_type_get_type(pFieldType);
	switch (type)
	{
	case MONO_TYPE_R4: return EFieldType::Float;
	case MONO_TYPE_I4: return EFieldType::Int;
	case MONO_TYPE_U4: return EFieldType::UnsignedInt;
	case MONO_TYPE_STRING: return EFieldType::String;
	case MONO_TYPE_VALUETYPE:
	{
		const char* sName = mono_type_get_name(pFieldType);
		if (strcmp(sName, "Xaloc.Vector2") == 0) return EFieldType::Vec2;
		if (strcmp(sName, "Xaloc.Vector3") == 0) return EFieldType::Vec3;
		if (strcmp(sName, "Xaloc.Vector4") == 0) return EFieldType::Vec4;
	}
	}
	return EFieldType::None;
}

MonoObject* GetMonoObjectInstance(const SEntityInstance& instance)
{
	return mono_gchandle_get_target(instance.Handle);
}

}