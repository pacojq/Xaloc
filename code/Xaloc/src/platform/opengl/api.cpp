#include "xapch.h"
#include "platform/opengl/api.h"
#include <glad/glad.h>


namespace Xaloc
{

void ZRendererAPIOpenGL::Init()
{
	XA_CORE_INFO("Initializing OpenGL Renderer API");
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_STENCIL_TEST);
		
	glEnable(GL_DEPTH_TEST);
}

void ZRendererAPIOpenGL::SetViewport(
	uint32 nX,
	uint32 nY,
	uint32 nWidth,
	uint32 nHeight)
{
	glViewport(nX, nY, nWidth, nHeight);
}

void ZRendererAPIOpenGL::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void ZRendererAPIOpenGL::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ZRendererAPIOpenGL::DrawIndexed(uint32 nIndexCount)
{
	glDrawElements(GL_TRIANGLES, nIndexCount, GL_UNSIGNED_INT, nullptr);
	glad_glBindTexture(GL_TEXTURE_2D, 0);
}

void ZRendererAPIOpenGL::DrawBlit()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

}