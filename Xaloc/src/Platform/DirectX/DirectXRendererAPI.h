#pragma once

#include "Xaloc/Renderer/RendererAPI.h"

namespace Xaloc {

	class DirectXRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(uint32_t indexCount) override;

	private:
		glm::vec4 m_ClearColor;
	};

}