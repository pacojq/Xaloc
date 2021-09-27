#include "xapch.h"
#include "DirectXFramebuffer.h"

#include "DirectXContext.h"
#include "Xaloc/Core/Application.h"

namespace Xaloc {

	DirectXFramebuffer::DirectXFramebuffer(const FramebufferSpec& spec)
		: m_Spec(spec)
	{
		m_Context = (DirectXContext*)Application::Get().GetWindow().GetContext();

		Invalidate();
	}

	
	DirectXFramebuffer::~DirectXFramebuffer()
	{
	}


	void DirectXFramebuffer::Invalidate()
	{
		D3D11_TEXTURE2D_DESC tDesc;
		ZeroMemory(&tDesc, sizeof(tDesc));

		tDesc.ArraySize = 1;
		tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		tDesc.CPUAccessFlags = 0;
		tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tDesc.Height = m_Spec.Height;
		tDesc.MipLevels = 1;
		tDesc.SampleDesc.Count = 1;
		tDesc.Usage = D3D11_USAGE_DEFAULT;
		tDesc.Width = m_Spec.Width;

		DX_CALL_HR(m_Context->GetDevice()->CreateTexture2D(&tDesc, nullptr, m_CopyTexture.GetAddressOf()));

		D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(sDesc));
		sDesc.Format = tDesc.Format;
		sDesc.Texture2D.MipLevels = tDesc.MipLevels;
		sDesc.Texture2D.MostDetailedMip = 0;
		sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		if (m_FrameData)
			m_FrameData.Reset();

		DX_CALL_HR(m_Context->GetDevice()->CreateShaderResourceView(m_CopyTexture.Get(), &sDesc, m_FrameData.GetAddressOf()));

		if (m_RenderTargetView)
			m_RenderTargetView.Reset();

		DX_CALL_HR(m_Context->GetDevice()->CreateRenderTargetView(m_CopyTexture.Get(), nullptr, m_RenderTargetView.GetAddressOf()));

		m_FrameTexture.Reset();
		m_CopyTexture.Reset();
	}

	void DirectXFramebuffer::Bind()
	{
		defaultRenderTargetView = false;
		if (!m_Context)
			Invalidate();

		m_Context->GetContext()->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
	}

	void DirectXFramebuffer::Unbind()
	{
		defaultRenderTargetView = true;
		m_Context->GetContext()->OMSetRenderTargets(1, m_Context->GetMainRenderTargetView().GetAddressOf(), nullptr);
	}

	
	void DirectXFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Spec.Width = width;
		m_Spec.Height = height;
		Invalidate();
	}

}