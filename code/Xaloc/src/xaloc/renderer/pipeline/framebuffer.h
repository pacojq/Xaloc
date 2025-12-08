#pragma once
#include "xaloc/types.h"
#include "xaloc/core/core.h"

#include <glm/glm.hpp>

namespace Xaloc
{

struct SFramebufferDefinition
{
	uint32 nWidth;
	uint32 nHeight;
	uint32 nSamples = 1;
	glm::vec4 ClearColor;

	// TODO EFramebufferFormat Format;

	/// <summary>
	/// Whether we will render directly to the Swap Chain.
	/// </summary>
	bool bSwapChainTarget = false;
};

class ZFramebuffer
{
public:
	static ZFramebuffer* Create(
		const SFramebufferDefinition& definition);

	virtual ~ZFramebuffer() = default;

	virtual void Bind() = 0;

	virtual void Unbind() = 0;

	virtual void Resize(
		uint32 nWidth,
		uint32 nHeight) = 0;

	virtual uint32 GetColorAttachmentRendererID() const = 0;

	virtual const SFramebufferDefinition& GetDefinition() const = 0;
};

}