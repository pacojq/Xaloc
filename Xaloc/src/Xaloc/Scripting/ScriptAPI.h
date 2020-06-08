#pragma once

#include "Xaloc/Scripting/ScriptEngine.h"
#include "Xaloc/Core/Input/KeyCodes.h"

#include <glm/glm.hpp>

extern "C" {
	typedef struct _MonoString MonoString;
	typedef struct _MonoArray MonoArray;
}

namespace Xaloc { namespace Scripting {

	// Logging
	void Xaloc_Log_Fatal(MonoString* msg);
	void Xaloc_Log_Error(MonoString* msg);
	void Xaloc_Log_Warn(MonoString* msg);
	void Xaloc_Log_Info(MonoString* msg);
	void Xaloc_Log_Trace(MonoString* msg);

	// Input
	bool Xaloc_Input_IsKeyPressed(KeyCode key);

	// Entity
	void Xaloc_Entity_GetTransform(uint32_t sceneID, uint32_t entityID, glm::mat4* outTransform);
	void Xaloc_Entity_SetTransform(uint32_t sceneID, uint32_t entityID, glm::mat4* inTransform);
	void Xaloc_Entity_CreateComponent(uint32_t sceneID, uint32_t entityID, void* type);
	bool Xaloc_Entity_HasComponent(uint32_t sceneID, uint32_t entityID, void* type);


	// Tag Component
	MonoString* Xaloc_TagComponent_GetTag(uint32_t sceneID, uint32_t entityID);
	void Xaloc_TagComponent_SetTag(uint32_t sceneID, uint32_t entityID, MonoString* inTag);
} }