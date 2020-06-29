#include "xapch.h"
#include "RenderPass.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLRenderPass.h"
// TODO #include "Platform/Vulkan/VulkanRenderPass.h"

namespace Xaloc {


	Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLRenderPass>(spec);

		// TODO case RendererAPI::API::Vulkan:
		// TODO 	return CreateRef<VulkanRenderPass>(spec);
		}

		XA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}