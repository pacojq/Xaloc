#include "xapch.h"
#include "platform/opengl/framebuffer.h"
#include "xaloc/debugging/assert.h"
#include <glad/glad.h>


namespace Xaloc
{

ZFramebufferOpenGL::ZFramebufferOpenGL(
	const SFramebufferDefinition& definition)
	: m_Definition(definition)
{
	Invalidate();
}

ZFramebufferOpenGL::~ZFramebufferOpenGL()
{
	glDeleteFramebuffers(1, &m_RendererID);
	glDeleteTextures(1, &m_ColorAttachment);
	glDeleteTextures(1, &m_DepthAttachment);
}

void ZFramebufferOpenGL::Invalidate()
{
	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	glCreateFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	// COLOR TEXTURE
	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8 /* TODO mSpec.Format */, m_Definition.nWidth, m_Definition.nHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	// Linear up-scaling and down-scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

	// DEPTH TEXTURE
	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Definition.nWidth, m_Definition.nHeight,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

	// Check status
	XA_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

	// Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ZFramebufferOpenGL::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	glViewport(0, 0, m_Definition.nWidth, m_Definition.nHeight);
}

void ZFramebufferOpenGL::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ZFramebufferOpenGL::Resize(
	uint32 nWidth,
	uint32 nHeight)
{
	m_Definition.nWidth = nWidth;
	m_Definition.nHeight = nHeight;
	Invalidate();
}

}