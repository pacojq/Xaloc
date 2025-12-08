#pragma once
#include "xaloc/types.h"
#include "xaloc/renderer/pipeline/pipeline.h"


namespace Xaloc
{

class ZRenderPipelineOpenGL : public ZRenderPipeline
{
public:
	ZRenderPipelineOpenGL(const SRenderPipelineDefinition& definition);
	virtual ~ZRenderPipelineOpenGL();

//		virtual SRenderPipelineDefinition& GetSpecification() { return m_Specification; }
	virtual const SRenderPipelineDefinition* GetDefinition() const { return &m_Definition; }

	virtual void Invalidate() override;

	virtual void Bind() override;

private:
	SRenderPipelineDefinition m_Definition;
	uint32 m_idVertexArray = 0;
};

}