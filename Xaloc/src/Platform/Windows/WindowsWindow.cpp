#include "xapch.h"

#include "Xaloc/Core/Window.h"
#include "Xaloc/Renderer/Renderer.h"

#include "DirectX/WindowsDirectXWindow.h"
#include "GLFW/WindowsGlfwWindow.h"


namespace Xaloc {

	float Window::s_HighDPIScaleFactor = 1.0f;
	
	Window* Window::Create(const WindowProps& props)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			case RendererAPI::API::Vulkan:
				return new WindowsGlfwWindow(props);

			case RendererAPI::API::DirectX:
				return new WindowsDirectXWindow(props);

		}
		XA_ASSERT(false, "Unknown render API!");
		return nullptr;
	}

}
