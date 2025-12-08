#pragma once
#include "xaloc/types.h"
#include "xaloc/debugging/assert.h"

/*
	A vertex buffer is used to store the vertices of a mesh.
	They can be ordered using a given VertexBufferLayout.

	For example, a "hello-world" triangle would be like:

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,		0.8f, 0.0f, 0.2f, 1.0f,     // First vertex
			0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,     // Second vertex
			0.0f,  0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f,     // Third vertex
		};

		ZRef<Xaloc::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Xaloc::VertexBuffer::Create(vertices, sizeof(vertices)));

		Xaloc::VertexBufferLayout layout = {
			{ Xaloc::ShaderDataType::Float3, "a_Position" },
			{ Xaloc::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);

	We define a layout in which, for each vertex, we have a Float3 indicating
	its position and a Float4 storing its color.

	After having the vertices ready, we can go and set up the indices in the IndexBuffer:

		uint32_t indices[3] = { 0, 1, 2 };
		ZRef<Xaloc::IndexBuffer> indexBuffer;
		indexBuffer.reset(Xaloc::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

	And finally we can put them both together inside the VertexArray:

		m_VertexArray.reset(Xaloc::VertexArray::Create());
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
*/
namespace Xaloc
{

/// <summary>
/// Types of data that can be declared inside a vertex.
/// </summary>
enum class EVertexDataType : uint8
{
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
	Int,
	Int2,
	Int3,
	Int4,
	Bool,

	None  // Can be used as count
};

inline uint32 GetVertexDataTypeSize(EVertexDataType type)
{
	switch (type)
	{
		case EVertexDataType::Float:  return 4;
		case EVertexDataType::Float2: return 4 * 2;
		case EVertexDataType::Float3: return 4 * 3;
		case EVertexDataType::Float4: return 4 * 4;

		case EVertexDataType::Mat3:   return 4 * 3 * 3;
		case EVertexDataType::Mat4:   return 4 * 4 * 4;

		case EVertexDataType::Int:    return 4;
		case EVertexDataType::Int2:   return 4 * 2;
		case EVertexDataType::Int3:   return 4 * 3;
		case EVertexDataType::Int4:   return 4 * 4;

		case EVertexDataType::Bool:   return 1;
	}

	XA_CORE_ASSERT(false, "Unknown vertex data type");
	return 0;
}

inline uint32 GetVertexDataComponentCount(EVertexDataType eType)
{
	switch (eType)
	{
	case EVertexDataType::Float:  return 1;
	case EVertexDataType::Float2: return 2;
	case EVertexDataType::Float3: return 3;
	case EVertexDataType::Float4: return 4;

	case EVertexDataType::Mat3:   return 3 * 3;
	case EVertexDataType::Mat4:   return 4 * 4;

	case EVertexDataType::Int:    return 1;
	case EVertexDataType::Int2:   return 2;
	case EVertexDataType::Int3:   return 3;
	case EVertexDataType::Int4:   return 4;

	case EVertexDataType::Bool:   return 1;
	}

	XA_CORE_ASSERT(false, "Unknown vertex data type");
	return 0;
}

struct SVertexBufferElement
{
	std::string sName;
	EVertexDataType eType;
	uint32 nSize;
	bool bNormalized;

	SVertexBufferElement()
	{
	}

	SVertexBufferElement(
		EVertexDataType eType,
		const std::string& sName,
		bool bNormalized = false)
		: sName(sName)
		, eType(eType)
		, nSize(GetVertexDataTypeSize(eType))
		, bNormalized(bNormalized)
	{
	}
};

/*
	Determines how the data inside the VertexBuffer is structured.
	For each vertex, we will have one or many BufferElements.
*/
class ZVertexBufferLayout
{
public:

	ZVertexBufferLayout();

	ZVertexBufferLayout(const std::initializer_list<SVertexBufferElement> elements);

	inline uint32 GetStride() const { return m_nStride; }

//	inline const std::vector<VertexBufferElement>& GetElements() const { return m_aElements; }

	inline uint32 Size() const { return uint32(m_aElements.size()); }

	const SVertexBufferElement& ReadElement(int32 nElement) const;

	const uint32 GetOffset(int32 nElement) const;

	std::vector<SVertexBufferElement>::iterator begin()
	{
		return m_aElements.begin();
	}
	
	std::vector<SVertexBufferElement>::iterator end()
	{
		return m_aElements.end();
	}

	std::vector<SVertexBufferElement>::const_iterator begin() const
	{
		return m_aElements.begin();
	}

	std::vector<SVertexBufferElement>::const_iterator end() const { return m_aElements.end(); }

private:
	std::vector<SVertexBufferElement> m_aElements;
	std::vector<uint32> m_aOffsets;

	uint32 m_nStride;
};

class ZVertexBuffer
{
public:
	virtual ~ZVertexBuffer() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void SetLayout(const ZVertexBufferLayout& layout) = 0;
	virtual const ZVertexBufferLayout& GetLayout() const = 0;

	virtual void SetData(const void* pData, uint32 nSize) = 0;

	static ZRef<ZVertexBuffer> Create(uint32 nSize);
	static ZRef<ZVertexBuffer> Create(
		float32* aVertices,
		uint32 nSize);
};

}