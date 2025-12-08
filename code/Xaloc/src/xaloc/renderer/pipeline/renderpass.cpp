#include "xapch.h"
#include "xaloc/debugging/assert.h"
#include "xaloc/renderer/renderer.h"
#include "xaloc/renderer/pipeline/renderpass.h"
#include "platform/opengl/renderpass.h"


namespace Xaloc
{

ZRenderPass* ZRenderPass::Create(
	const SRenderPassDefinition& definition)
{
	switch (RendererGetAPI())
	{
	case ERenderAPI::None:
		XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
		return nullptr;

	case ERenderAPI::OpenGL:
		return new ZRenderPassOpenGL(definition);
	}

	XA_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

}