#pragma once
#include "xaloc/renderer/api.h"
#include "xaloc/renderer/cameras/orthographic.h"
#include "xaloc/renderer/pipeline/RenderPass.h"
#include "xaloc/renderer/resource/Shader.h"
#include "xaloc/math/boundingbox.h"


namespace Xaloc
{

void RendererInit(ERenderAPI eAPI);

void RendererShutdown();

ERenderAPI RendererGetAPI();

ZRendererAPI* RendererGet();

void RendererSetViewport(
	uint32 nWidth,
	uint32 nHeight);

void RendererBeginScene(
	const ZOrthographicCamera& camera,
	const glm::mat4& transform);

void RendererEndScene();

void RendererBeginPass(
	const ZRenderPass* pRenderPass,
	bool clear = true);

void RendererEndPass();

}