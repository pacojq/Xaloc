#pragma once

// For use by Xaloc applications

#include "Xaloc/Core/Application.h"
#include "Xaloc/Core/Layer.h"
#include "Xaloc/Core/Log.h"

#include "Xaloc/Core/Timestep.h"


// --- Scene and ECS ----------------------------

#include "Xaloc/Scene/Scene.h"
#include "Xaloc/Scene/Components.h"
#include "Xaloc/Scene/Entity.h"


// --- Input ------------------------------------

#include "Xaloc/Core/Input/Input.h"
#include "Xaloc/Core/Input/KeyCodes.h"
#include "Xaloc/Core/Input/GamepadButtonCodes.h"
#include "Xaloc/Core/Input/MouseButtonCodes.h"


// --- Math -------------------------------------

#include "Xaloc/Core/Math/AABB.h"


// --- ImGui ------------------------------------

#include "imgui/imgui.h"
#include "Xaloc/ImGui/ImGuiLayer.h"


// --- Renderer ---------------------------------

#include "Xaloc/Renderer/OrthographicCameraController.h"

#include "Xaloc/Renderer/Renderer.h"
#include "Xaloc/Renderer/Renderer2D.h"
#include "Xaloc/Renderer/RenderCommand.h"

#include "Xaloc/Renderer/Buffer.h"
#include "Xaloc/Renderer/Shader.h"
#include "Xaloc/Renderer/Framebuffer.h"
#include "Xaloc/Renderer/Texture.h"
#include "Xaloc/Renderer/SubTexture2D.h"
#include "Xaloc/Renderer/VertexArray.h"

#include "Xaloc/Renderer/OrthographicCamera.h"

// ----------------------------------------------
