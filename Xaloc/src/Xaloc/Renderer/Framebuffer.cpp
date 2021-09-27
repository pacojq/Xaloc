#include "xapch.h"
#include "Framebuffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Platform/DirectX/DirectXFramebuffer.h"

namespace Xaloc {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpec& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			XA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(spec);

		case RendererAPI::API::DirectX:
			return CreateRef<DirectXFramebuffer>(spec);
		}

		XA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	
}