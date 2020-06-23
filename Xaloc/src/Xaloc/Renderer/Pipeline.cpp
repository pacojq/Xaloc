#include "xapch.h"
#include "Pipeline.h"

#include "Renderer.h"

// TODO #include "Platform/OpenGL/OpenGLPipeline.h"
// TODO #include "Platform/Vulkan/VulkanPipeline.h"

namespace Xaloc {


	Ref<Pipeline> Pipeline::Create(const PipelineSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
			return nullptr;

		// TODO case RendererAPI::API::OpenGL:
		// TODO 	return CreateRef<OpenGLPipeline>(spec);

		// TODO case RendererAPI::API::Vulkan:
		// TODO 	return CreateRef<VulkanPipeline>(spec);
		}

		XA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}