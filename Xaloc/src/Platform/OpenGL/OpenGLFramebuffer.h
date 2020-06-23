#pragma once

#include "Xaloc/Renderer/Framebuffer.h"

namespace Xaloc {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpec& spec);
		virtual ~OpenGLFramebuffer();
				
		/// <summary>
		/// Recreates the frame buffer.
		/// </summary>
		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual const FramebufferSpec& GetSpecification() const override { return m_Spec; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
		
		FramebufferSpec m_Spec;
	};
}