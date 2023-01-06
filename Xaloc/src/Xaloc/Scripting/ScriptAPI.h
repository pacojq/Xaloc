#pragma once

#include "Xaloc/Scripting/ScriptEngine.h"
#include "Xaloc/Core/Input/KeyCodes.h"

#include <glm/glm.hpp>

extern "C" {
	typedef struct _MonoString MonoString;
	typedef struct _MonoArray MonoArray;
}


namespace Xaloc {
	
/// <summary>
/// Namespace storing the C# scripting API.
/// </summary>
namespace Scripting {

	// ============================ LOGGING ============================ //
	
	void Xaloc_Log_Fatal(MonoString* msg);
	void Xaloc_Log_Error(MonoString* msg);
	void Xaloc_Log_Warn(MonoString* msg);
	void Xaloc_Log_Info(MonoString* msg);
	void Xaloc_Log_Trace(MonoString* msg);



	// ============================= INPUT ============================= //
	
	bool Xaloc_Input_IsKeyPressed(KeyCode key);




	// ============================ ENTITIES ============================ //

	void Xaloc_Entity_GetTransform(uint64_t entityID, glm::mat4* outTransform);
	void Xaloc_Entity_SetTransform(uint64_t entityID, glm::mat4* inTransform);
	void Xaloc_Entity_CreateComponent(uint64_t entityID, void* type);
	bool Xaloc_Entity_HasComponent(uint64_t entityID, void* type);




	// =========================== COMPONENTS =========================== //

	MonoString* Xaloc_TagComponent_GetTag(uint64_t entityID);
	void Xaloc_TagComponent_SetTag(uint64_t entityID, MonoString* inTag);
} }