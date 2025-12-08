#include "xapch.h"
#include "xaloc/renderer/renderer.h"
#include "xaloc/renderer/renderer2D.h"
#include "xaloc/renderer/pipeline/pipeline.h"
#include "xaloc/renderer/geometry/vertex.h"
#include "xaloc/renderer/geometry/index.h"
#include "platform/opengl/api.h"


namespace Xaloc
{

static ZRendererAPI* CreateRenderAPI(ERenderAPI eAPI)
{
	switch (eAPI)
	{
	case ERenderAPI::None:
		XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
		break;

	case ERenderAPI::OpenGL:
		return new ZRendererAPIOpenGL();
		break;

	default:
		XA_CORE_ASSERT(false, "Unknown RendererAPI!");
	}
	return nullptr;
}

struct SRendererData
{
	ERenderAPI eAPI;
	ZRendererAPI* pAPI;

	glm::mat4 mViewProjection;
	const ZRenderPass* pActiveRenderPass;
};

static SRendererData s_Data;


void RendererInit(ERenderAPI eAPI)
{
	s_Data.eAPI = eAPI;
	s_Data.pAPI = CreateRenderAPI(eAPI);
	s_Data.pAPI->Init();

	ZRenderer2D::Init();
}

void RendererShutdown()
{
	ZRenderer2D::Shutdown();
	delete s_Data.pAPI;
}

ERenderAPI RendererGetAPI()
{
	return s_Data.eAPI;
}

ZRendererAPI* RendererGet()
{
	return s_Data.pAPI;
}

void RendererSetViewport(uint32 nWidth, uint32 nHeight)
{
	s_Data.pAPI->SetViewport(0, 0, nWidth, nHeight);
}

void RendererBeginScene(
	ZOrthographicCamera& camera,
	const glm::mat4& transform)
{
	//                   Projection matrix      * View matrix
	glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
	s_Data.mViewProjection = viewProj;
}

void RendererEndScene()
{
	// TODO?
}

void RendererBeginPass(
	const ZRenderPass* pRenderPass,
	bool clear)
{
	XA_CORE_ASSERT(pRenderPass, "Render pass cannot be null!");

	s_Data.pActiveRenderPass = pRenderPass;

	ZFramebuffer* pTarget = pRenderPass->GetDefinition().pTarget;
	pTarget->Bind();

	if (clear)
	{
		const glm::vec4& clearColor = pTarget->GetDefinition().ClearColor;
		s_Data.pAPI->SetClearColor(clearColor);
		s_Data.pAPI->Clear();
	}
}

void RendererEndPass()
{
	XA_CORE_ASSERT(s_Data.pActiveRenderPass, "No active render pass!");
	s_Data.pActiveRenderPass->GetDefinition().pTarget->Unbind();
	s_Data.pActiveRenderPass = nullptr;
}

}