#include "xapch.h"
#include "platform/opengl/renderpass.h"


namespace Xaloc
{

ZRenderPassOpenGL::ZRenderPassOpenGL(
	const SRenderPassDefinition& spec)
	: m_Specification(spec)
{
}

ZRenderPassOpenGL::~ZRenderPassOpenGL()
{
}

}