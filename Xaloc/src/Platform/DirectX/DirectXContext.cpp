#include "xapch.h"
#include "DirectXContext.h"


namespace Xaloc {


	DirectXContext::DirectXContext(HWND windowHandle, uint32_t width, uint32_t height)
		: m_WindowHandle(windowHandle), m_Width(width), m_Height(height)
	{
		//XA_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	DirectXContext::~DirectXContext()
	{
	}


	void DirectXContext::Init()
	{
		XA_CORE_TRACE("DirectX Context initializing...");

		D3D_FEATURE_LEVEL dx11 = D3D_FEATURE_LEVEL_11_0;

		// SwapChain Descriptor
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = m_Width;
		swapChainDesc.BufferDesc.Height = m_Height;
		swapChainDesc.BufferDesc.RefreshRate = { 0, 0 };
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc = { 1, 0 };
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = m_WindowHandle;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Set up viewport
		m_Viewport.Width = m_Width;
		m_Viewport.Height = m_Height;
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.MinDepth = 0; 
		m_Viewport.MaxDepth = 1;


		// Enable debug layer
		UINT deviceCreateFlags = 0;
#ifdef XA_DEBUG
		deviceCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// Create Device and SwapChain
		DX_CALL_HR(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			deviceCreateFlags,
			&dx11, 1,
			D3D11_SDK_VERSION,
			&swapChainDesc, &m_SwapChain, &m_Device,
			nullptr,
			&m_DeviceContext
		));

		// Main RenderTargetView
		CreateMainRenderTargetView();


		D3D11_RASTERIZER_DESC rDesc = {};
		rDesc.CullMode = D3D11_CULL_NONE;
		rDesc.FillMode = D3D11_FILL_SOLID;

		ID3D11RasterizerState* state;
		DX_CALL_HR(m_Device->CreateRasterizerState(&rDesc, &state));

		m_DeviceContext->RSSetState(state);
		m_DeviceContext->RSSetViewports(1, &m_Viewport);

		state->Release();



		XA_CORE_TRACE("DirectX Context ready!");
	}



	void DirectXContext::CreateMainRenderTargetView()
	{
		ID3D11Texture2D* backbuffer;

		DX_CALL_HR(m_SwapChain->GetBuffer(0, __uuidof(backbuffer), (void**)&backbuffer));
		DX_CALL_HR(m_Device->CreateRenderTargetView(backbuffer, NULL, &m_MainRenderTargetView));

		backbuffer->Release();

		ID3D11RenderTargetView* views = { m_MainRenderTargetView.Get() };

		m_DeviceContext->OMSetRenderTargets(1, &views, nullptr);
	}



	void DirectXContext::SwapBuffers()
	{
		bool vSync = true; // TODO: vSync:  Application::Get().GetWindow().IsVSync();
		m_SwapChain->Present(vSync, 0);
	}

}