#pragma once
#include "xaloc/renderer/pipeline/framebuffer.h"


namespace Xaloc
{

class ZFramebufferOpenGL : public ZFramebuffer
{
public:
	ZFramebufferOpenGL(
		const SFramebufferDefinition& spec);

	virtual ~ZFramebufferOpenGL() override;

	/// <summary>
	/// Recreates the frame buffer.
	/// </summary>
	void Invalidate();

	virtual void Bind() override;
	virtual void Unbind() override;

	virtual void Resize(
		uint32 nWidth,
		uint32 nHeight) override;

	virtual uint32 GetColorAttachmentRendererID() const override
	{
		return m_ColorAttachment;
	}

	virtual const SFramebufferDefinition& GetDefinition() const override
	{
		return m_Definition;
	}

private:
	uint32 m_RendererID = 0;
	uint32 m_ColorAttachment = 0;
	uint32 m_DepthAttachment = 0;

	SFramebufferDefinition m_Definition;
};

}