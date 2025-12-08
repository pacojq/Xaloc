#include "xapch.h"
#include "xaloc/debugging/assert.h"
#include "xaloc/renderer/geometry/index.h"
#include "xaloc/renderer/renderer.h"
#include "platform/opengl/index.h"


namespace Xaloc
{

ZRef<ZIndexBuffer> ZIndexBuffer::Create(
	uint32* aIndices,
	uint32 nCount)
{
	switch (RendererGetAPI())
	{
	case ERenderAPI::None:
		XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
		return nullptr;

	case ERenderAPI::OpenGL:
		return CreateRef<ZIndexBufferOpenGL>(aIndices, nCount);
	}

	XA_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

}