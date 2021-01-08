#include "xapch.h"

// TODO #if XA_IMGUI_GLFW
// TODO #if XA_IMGUI_DIRECTX

// This file exists just to properly build ImGui

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "examples/imgui_impl_opengl3.cpp"
#include "examples/imgui_impl_vulkan.cpp"
#include "examples/imgui_impl_glfw.cpp"
#include "examples/imgui_impl_dx11.cpp"
#include "examples/imgui_impl_win32.cpp"