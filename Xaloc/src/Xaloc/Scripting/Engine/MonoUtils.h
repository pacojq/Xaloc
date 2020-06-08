#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>

namespace Xaloc {

	class MonoUtils
	{
	public:
		static MonoMethod* GetMethod(MonoImage* image, const std::string& methodDesc);
		static MonoObject* CallMethod(MonoObject* object, MonoMethod* method, void** params = nullptr);

		static void PrintClassMethods(MonoClass* monoClass);
		static void PrintClassProperties(MonoClass* monoClass);
	};
	
}