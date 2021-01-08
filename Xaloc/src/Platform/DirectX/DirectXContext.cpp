#include "xapch.h"
#include "DirectXContext.h"


namespace Xaloc {


	DirectXContext::DirectXContext(HWND windowHandle, uint32_t width, uint32_t height)
		: m_WindowHandle(windowHandle), m_Width(width), m_Height(height)
	{
		//XA_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void DirectXContext::Init()
	{
		XA_CORE_TRACE("DirectX Context initializing...");


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

		// Enable debug layer
		UINT deviceCreateFlags = 0;
		#ifdef HZ_DEBUG
		deviceCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

		// Create Device and SwapChain
		DX_CALL_HR(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			deviceCreateFlags,
			nullptr, 0,
			D3D11_SDK_VERSION,
			&swapChainDesc, &m_SwapChain, &m_Device,
			nullptr,
			&m_DeviceContext
		));

		// Main RenderTargetView
		//CreateMainRenderTargetView();

		XA_CORE_TRACE("DirectX Context ready!");
	}


	void DirectXContext::SwapBuffers()
	{
	}

}