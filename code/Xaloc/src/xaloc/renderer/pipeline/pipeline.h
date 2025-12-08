#pragma once
#include "xaloc/core/core.h"
#include "xaloc/renderer/resource/shader.h"
#include "xaloc/renderer/geometry/vertex.h"


namespace Xaloc
{

struct SRenderPipelineDefinition
{
	ZRef<ZShader> Shader;
	ZVertexBufferLayout Layout;
};

class ZRenderPipeline
{
public:
	virtual ~ZRenderPipeline() = default;

	virtual void Invalidate() = 0;

	virtual void Bind() = 0;

	virtual const SRenderPipelineDefinition* GetDefinition() const = 0;

	static ZRef<ZRenderPipeline> Create(
		const SRenderPipelineDefinition& definition);
};

}