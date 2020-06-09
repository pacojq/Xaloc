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

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual const FramebufferSpec& GetSpec() const override { return m_Spec; }

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
		
		FramebufferSpec m_Spec;
	};
}