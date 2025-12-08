#include "xapch.h"
#include "platform/opengl/vertex.h"
#include <glad/glad.h>


namespace Xaloc
{

ZVertexBufferOpenGL::ZVertexBufferOpenGL(uint32 nSize)
{
	glCreateBuffers(1, &m_idRenderer);
	glBindBuffer(GL_ARRAY_BUFFER, m_idRenderer);
	glBufferData(GL_ARRAY_BUFFER, nSize, nullptr, GL_DYNAMIC_DRAW);
}

ZVertexBufferOpenGL::ZVertexBufferOpenGL(
	float32* aVertices,
	uint32 nSize)
{
	glCreateBuffers(1, &m_idRenderer);
	glBindBuffer(GL_ARRAY_BUFFER, m_idRenderer);
	glBufferData(GL_ARRAY_BUFFER, nSize, aVertices, GL_STATIC_DRAW);
}

ZVertexBufferOpenGL::~ZVertexBufferOpenGL()
{
	glDeleteBuffers(1, &m_idRenderer);
}

void ZVertexBufferOpenGL::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_idRenderer);
}

void ZVertexBufferOpenGL::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ZVertexBufferOpenGL::SetData(
	const void* pData,
	uint32 nSize)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_idRenderer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nSize, pData);
}

}