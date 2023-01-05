#include "xapch.h"
#include "IndexBuffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Xaloc {

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		XA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}