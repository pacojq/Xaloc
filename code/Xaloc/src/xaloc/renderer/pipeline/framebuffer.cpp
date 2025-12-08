#include "xapch.h"
#include "xaloc/debugging/assert.h"
#include "xaloc/renderer/renderer.h"
#include "xaloc/renderer/pipeline/framebuffer.h"
#include "platform/opengl/framebuffer.h"


namespace Xaloc
{

ZFramebuffer* ZFramebuffer::Create(
	const SFramebufferDefinition& definition)
{
	switch (RendererGetAPI())
	{
	case ERenderAPI::None:
		XA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		return nullptr;

	case ERenderAPI::OpenGL:
		return new ZFramebufferOpenGL(definition);
	}

	XA_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

}