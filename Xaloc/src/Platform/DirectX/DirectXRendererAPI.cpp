#include "xapch.h"
#include "DirectXRendererAPI.h"

#include "DirectXContext.h"

#include "Xaloc/Core/Application.h"
#include "glm/gtc/type_ptr.hpp"

namespace Xaloc {

	void DirectXRendererAPI::Init()
	{
		XA_CORE_INFO("Initializing DirectX Renderer API");
		//m_Context = (DirectXContext*)Application::Get().GetWindow().GetContext();
		
	}

	void DirectXRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		// TODO
	}

	void DirectXRendererAPI::SetClearColor(const glm::vec4& color)
	{
		m_ClearColor = color;
	}

	void DirectXRendererAPI::Clear()
	{
		DirectXContext* ctx = (DirectXContext*)Application::Get().GetWindow().GetContext();

		ID3D11RenderTargetView* views[1];
		ctx->GetContext()->OMGetRenderTargets(1, views, nullptr);

		if (views[0] == nullptr)
			ctx->GetContext()->ClearRenderTargetView(ctx->GetMainRenderTargetView().Get(), glm::value_ptr(m_ClearColor));
		else
			ctx->GetContext()->ClearRenderTargetView(views[0], glm::value_ptr(m_ClearColor));

		if (views[0] != nullptr)
			views[0]->Release();
	}

	void DirectXRendererAPI::DrawIndexed(uint32_t indexCount)
	{
		DirectXContext* ctx = (DirectXContext*)Application::Get().GetWindow().GetContext();


		//uint32_t count = indices ? indices : batch->GetIndexBuffer()->GetCount();
		ctx->GetContext()->DrawIndexed(indexCount, 0, 0);
	}

}