#pragma once

#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"

namespace Xaloc {

	/// <summary>
	/// RendererCommand is just a layer that abstracts the actual RendererAPI.
	/// Different Render Commands will be called by Xaloc's Renderers.
	/// </summary>
	class RenderCommand
	{
	public:

		static void Init(RendererAPI::API api)
		{
			switch (api)
			{
			case RendererAPI::API::None:
				XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
				break;

			case RendererAPI::API::OpenGL:
				s_RendererAPI = CreateScope<OpenGLRendererAPI>();
				break;

			case RendererAPI::API::Vulkan:
				s_RendererAPI = CreateScope<VulkanRendererAPI>();
				break;

			default:
				XA_CORE_ASSERT(false, "Unknown RendererAPI!");
			}
			
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(indexCount);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}