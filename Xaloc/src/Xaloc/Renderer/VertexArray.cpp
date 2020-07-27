#include "xapch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/Vulkan/VulkanVertexArray.h"

namespace Xaloc {


	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL	:
				return CreateRef<OpenGLVertexArray>();

			case RendererAPI::API::Vulkan:
				return CreateRef<VulkanVertexArray>();
		}

		XA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}