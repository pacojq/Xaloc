#pragma once
#include "xaloc/core/core.h"
#include "xaloc/renderer/pipeline/framebuffer.h"


namespace Xaloc
{

struct SRenderPassDefinition
{
	ZFramebuffer* pTarget;
};

class ZRenderPass
{
public:
	static ZRenderPass* Create(
		const SRenderPassDefinition& definition);

	virtual ~ZRenderPass() = default;

	virtual const SRenderPassDefinition& GetDefinition() const = 0;
};

}