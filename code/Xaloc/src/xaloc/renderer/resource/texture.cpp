#include "xapch.h"
#include "xaloc/debugging/assert.h"
#include "xaloc/renderer/resource/texture.h"
#include "xaloc/renderer/renderer.h"
#include "platform/opengl/texture.h"


namespace Xaloc
{

ZRef<ZTexture2D> ZTexture2D::Create(uint32 nWidth, uint32 nHeight)
{
	switch (RendererGetAPI())
	{
		case ERenderAPI::None:
			XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
			return nullptr;

		case ERenderAPI::OpenGL:
			return CreateRef<ZTexture2DOpenGL>(nWidth, nHeight);
	}

	XA_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

ZRef<ZTexture2D> ZTexture2D::Create(const char* sPath)
{
	switch (RendererGetAPI())
	{
		case ERenderAPI::None:
			XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
			return nullptr;

		case ERenderAPI::OpenGL:
			return CreateRef<ZTexture2DOpenGL>(sPath);
	}

	XA_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

}