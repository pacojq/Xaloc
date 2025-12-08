#pragma once
#include "xaloc/renderer/pipeline/renderpass.h"


namespace Xaloc
{

class ZRenderPassOpenGL : public ZRenderPass
{
public:
	ZRenderPassOpenGL(const SRenderPassDefinition& spec);
	virtual ~ZRenderPassOpenGL();

	virtual const SRenderPassDefinition& GetDefinition() const override { return m_Specification; }
private:
	SRenderPassDefinition m_Specification;
};

}
