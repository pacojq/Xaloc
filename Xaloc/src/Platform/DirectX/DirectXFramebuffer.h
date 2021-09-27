#pragma once

#include "Xaloc/Renderer/Framebuffer.h"

#include "DirectXContext.h"

#include <wrl.h>

namespace Xaloc {

	class DirectXFramebuffer : public Framebuffer
	{
	public:
		DirectXFramebuffer(const FramebufferSpec& spec);
		virtual ~DirectXFramebuffer() override;
				
		/// <summary>
		/// Recreates the frame buffer.
		/// </summary>
		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return 0; } // TODO

		virtual const FramebufferSpec& GetSpecification() const override { return m_Spec; }

	private:
		DirectXContext* m_Context = nullptr;

		bool defaultRenderTargetView = true;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_FrameTexture;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_CopyTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_FrameData;

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		
		FramebufferSpec m_Spec;
	};
}