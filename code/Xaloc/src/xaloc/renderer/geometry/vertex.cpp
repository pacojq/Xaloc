#include "xapch.h"
#include "xaloc/types.h"
#include "xaloc/debugging/assert.h"
#include "xaloc/renderer/geometry/vertex.h"
#include "xaloc/renderer/renderer.h"
#include "platform/opengl/vertex.h"


namespace Xaloc
{

static void ComputeOffsetsAndStride(
	std::vector<uint32>& rOffsets,
	uint32& rStride,
	const std::vector<SVertexBufferElement>& aElements)
{
	const uint32 nElementCount = uint32(aElements.size());

	rOffsets.clear();
	rOffsets.reserve(nElementCount);
	rStride = 0;
	uint32 nOffset = 0;

	for (uint32 nElement = 0; nElement < nElementCount; ++nElement)
	{
		const SVertexBufferElement& element = aElements[nElement];

		rOffsets.push_back(nOffset);
		nOffset += element.nSize;
		rStride += element.nSize;
	}
}

ZVertexBufferLayout::ZVertexBufferLayout()
	: m_aElements()
	, m_aOffsets()
	, m_nStride(0)
{
}

ZVertexBufferLayout::ZVertexBufferLayout(const std::initializer_list<SVertexBufferElement> elements)
	: m_aElements(elements)
{
	ComputeOffsetsAndStride(m_aOffsets, m_nStride, m_aElements);
}

const SVertexBufferElement& ZVertexBufferLayout::ReadElement(
	int32 nElement) const
{
	return m_aElements[nElement];
}

const uint32 ZVertexBufferLayout::GetOffset(int32 nElement) const
{
	return m_aOffsets[nElement];
}

ZRef<ZVertexBuffer> ZVertexBuffer::Create(uint32 size)
{
	switch (RendererGetAPI())
	{
	case ERenderAPI::None:
		XA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		return nullptr;

	case ERenderAPI::OpenGL:
		return CreateRef<ZVertexBufferOpenGL>(size);
	}

	XA_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
	
ZRef<ZVertexBuffer> ZVertexBuffer::Create(
	float32* aVertices,
	uint32 nSize)
{
	switch (RendererGetAPI())
	{
	case ERenderAPI::None:
		XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
		return nullptr;

	case ERenderAPI::OpenGL:
		return CreateRef<ZVertexBufferOpenGL>(aVertices, nSize);
	}

	XA_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

}