#pragma once
#include "xaloc/scene/entity.h"
#include <mono/metadata/metadata.h>
#include <mono/metadata/object-forward.h>


namespace Scripting
{

using ZFunctionHasComponent = std::function<bool(const Xaloc::ZEntity&)>;
using ZFunctionCreateComponent = std::function<void(Xaloc::ZEntity&)>;

/// <summary>
/// Used to register the implementation of the C# Xaloc methods
/// that the client scripting application is going to call.
/// </summary>
void RegisterEngineMethods(MonoImage* pAssemblyImage);

ZFunctionHasComponent FindFunctionHasComponent(MonoType* pType);

ZFunctionCreateComponent FindFunctionCreateComponent(MonoType* pType);

}
