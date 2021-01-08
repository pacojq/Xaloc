#pragma once

#include "Xaloc/Renderer/GraphicsContext.h"

#include <wrl.h>
#include <d3d11.h>
#include <dxgidebug.h>
#include <DirectXMath.h>

namespace Xaloc {


#ifdef HZ_DEBUG
	#define DX_CALL_HR(x) { dxGetInfoQueue().Clear(); HRESULT _hr; if(FAILED(_hr = (x))) { HZ_CORE_ASSERT(false, "Failed DirectX call: {0}", dxGetInfoQueue().Get()); } }
	#define DX_CALL_VOID(x) { dxGetInfoQueue().Clear(); x; { auto _debugString = dxGetInfoQueue().Get(); HZ_CORE_ASSERT(_debugString.empty(), "Failed DirectX Call: {0}", _debugString); } }
#else
	#define DX_CALL_HR(x) x
	#define DX_CALL_VOID(x) x
#endif


	class DirectXContext : public GraphicsContext
	{
	public:
		DirectXContext(HWND windowHandle, uint32_t width, uint32_t height);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		HWND m_WindowHandle;
		uint32_t m_Width, m_Height;

		// TODO mayba migrate stuff to a swap chain class
		// DirectX Objects
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_MainRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_MainDepthStencilView;
	};

}