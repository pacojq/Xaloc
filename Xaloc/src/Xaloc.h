#pragma once


// For use by Xaloc applications

#include "Xaloc/Core/Application.h"
#include "Xaloc/Core/Layer.h"
#include "Xaloc/Core/Log.h"

#include "Xaloc/Core/Timestep.h"

#include "Xaloc/Debugging/Assert.h"


// --- Scene and ECS ----------------------------

#include "Xaloc/Scene/Scene.h"
#include "Xaloc/Scene/Components.h"
#include "Xaloc/Scene/Entity.h"


// --- Input ------------------------------------

#include "Xaloc/Core/Input/Input.h"
#include "Xaloc/Core/Input/KeyCodes.h"
#include "Xaloc/Core/Input/GamepadButtonCodes.h"
#include "Xaloc/Core/Input/MouseButtonCodes.h"


// --- Assets -------------------------------------

#include "Xaloc/Core/Assets/AssetManager.h"


// --- Math -------------------------------------

#include "Xaloc/Math/Math.h"
#include "Xaloc/Math/AABB.h"


// --- ImGui ------------------------------------

#include "imgui/imgui.h"
#include "Xaloc/ImGui/ImGuiLayer.h"


// --- Renderer ---------------------------------

#include "Xaloc/Renderer/OrthographicCameraController.h"

#include "Xaloc/Renderer/Renderer.h"
#include "Xaloc/Renderer/Renderer2D.h"
#include "Xaloc/Renderer/RenderCommand.h"

#include "Xaloc/Renderer/VertexBuffer.h"
#include "Xaloc/Renderer/IndexBuffer.h"
#include "Xaloc/Renderer/Shader.h"
#include "Xaloc/Renderer/Texture.h"
#include "Xaloc/Renderer/SubTexture2D.h"

#include "Xaloc/Renderer/Pipeline.h"
#include "Xaloc/Renderer/RenderPass.h"
#include "Xaloc/Renderer/Framebuffer.h"

#include "Xaloc/Renderer/OrthographicCamera.h"

// ----------------------------------------------
