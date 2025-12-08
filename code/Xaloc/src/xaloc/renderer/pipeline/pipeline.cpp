#include "xapch.h"
#include "xaloc/debugging/assert.h"
#include "xaloc/renderer/pipeline/pipeline.h"
#include "xaloc/renderer/renderer.h"
#include "platform/opengl/pipeline.h"


namespace Xaloc
{

ZRef<ZRenderPipeline> ZRenderPipeline::Create(
	const SRenderPipelineDefinition& definition)
{
	switch (RendererGetAPI())
	{
	case ERenderAPI::None:
		XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
		return nullptr;

	case ERenderAPI::OpenGL:
		return CreateRef<ZRenderPipelineOpenGL>(definition);
	}

	XA_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

}