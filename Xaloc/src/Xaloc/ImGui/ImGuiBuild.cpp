#include "xapch.h"

// This file exists just to properly build ImGui

#ifdef XA_IMGUI_GLFW
	#define IMGUI_IMPL_OPENGL_LOADER_GLAD
	#include "examples/imgui_impl_opengl3.cpp"
	#include "examples/imgui_impl_vulkan.cpp"
	#include "examples/imgui_impl_glfw.cpp"
#endif
#ifdef XA_IMGUI_DIRECTX
	#include "examples/imgui_impl_dx11.cpp"
	#include "examples/imgui_impl_win32.cpp"
#endif