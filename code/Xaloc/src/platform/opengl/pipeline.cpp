#include "xapch.h"
#include "platform/opengl/pipeline.h"
#include "xaloc/debugging/assert.h"
#include "xaloc/renderer/renderer.h"
#include "xaloc/renderer/geometry/vertex.h"
#include <glad/glad.h>


namespace Xaloc
{

static GLenum ShaderDataTypeToOpenGLBaseType(EVertexDataType eType)
{
	switch (eType)
	{
	case EVertexDataType::Float:  return GL_FLOAT;
	case EVertexDataType::Float2: return GL_FLOAT;
	case EVertexDataType::Float3: return GL_FLOAT;
	case EVertexDataType::Float4: return GL_FLOAT;

	case EVertexDataType::Mat3:   return GL_FLOAT;
	case EVertexDataType::Mat4:   return GL_FLOAT;

	case EVertexDataType::Int:    return GL_INT;
	case EVertexDataType::Int2:   return GL_INT;
	case EVertexDataType::Int3:   return GL_INT;
	case EVertexDataType::Int4:   return GL_INT;

	case EVertexDataType::Bool:   return GL_BOOL;
	}

	XA_CORE_ASSERT(false, "Unknown ShaderDataType");
	return 0;
}

ZRenderPipelineOpenGL::ZRenderPipelineOpenGL(const SRenderPipelineDefinition& definition)
	: m_Definition(definition)
{
	Invalidate();
}

ZRenderPipelineOpenGL::~ZRenderPipelineOpenGL()
{
	GLuint rendererID = m_idVertexArray;
	glDeleteVertexArrays(1, &rendererID);
}

void ZRenderPipelineOpenGL::Invalidate()
{
	XA_CORE_ASSERT(m_Definition.Layout.Size(), "Layout is empty!");

	if (m_idVertexArray)
	{
		glDeleteVertexArrays(1, &m_idVertexArray);
	}

	glGenVertexArrays(1, &m_idVertexArray);
	glBindVertexArray(m_idVertexArray);

	// TODO Bind() ?

	glBindVertexArray(0);
}

void ZRenderPipelineOpenGL::Bind()
{
	const uint32 nElementCount = m_Definition.Layout.Size();
	const uint32 nStride = m_Definition.Layout.GetStride();
	XA_CORE_ASSERT(nElementCount, "Vertex buffer has no layout!");

	glBindVertexArray(m_idVertexArray);
	//vertexBuffer->Bind();

	uint32 nAttribute = 0;
	for (uint32 nElement = 0; nElement < nElementCount; ++nElement)
	{
		const SVertexBufferElement& element = m_Definition.Layout.ReadElement(nElement);
		const uint32 nOffset = m_Definition.Layout.GetOffset(nElement);

		glEnableVertexAttribArray(nAttribute);
		glVertexAttribPointer(
			nAttribute,
			GetVertexDataComponentCount(element.eType),
			ShaderDataTypeToOpenGLBaseType(element.eType),
			element.bNormalized ? GL_TRUE : GL_FALSE,
			nStride,
			(const void*)(intptr_t)nOffset);
		nAttribute++;
	}
}

}