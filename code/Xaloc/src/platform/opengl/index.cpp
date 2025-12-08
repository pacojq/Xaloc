#include "xapch.h"
#include "platform/opengl/index.h"
#include <glad/glad.h>


namespace Xaloc
{

ZIndexBufferOpenGL::ZIndexBufferOpenGL(
	uint32* aIndices,
	uint32 nCount)
	: m_nCount(nCount)
{
	glCreateBuffers(1, &m_idRenderer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idRenderer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nCount * sizeof(uint32), aIndices, GL_STATIC_DRAW);
}

ZIndexBufferOpenGL::~ZIndexBufferOpenGL()
{
	glDeleteBuffers(1, &m_idRenderer);
}

void ZIndexBufferOpenGL::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idRenderer);
}

void ZIndexBufferOpenGL::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}