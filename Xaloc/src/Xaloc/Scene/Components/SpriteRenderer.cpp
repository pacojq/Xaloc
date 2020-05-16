#include "xapch.h"
#include "SpriteRenderer.h"

#include "Xaloc/Renderer/Renderer2D.h"

namespace Xaloc {

	SpriteRenderer::SpriteRenderer(const std::string& filename)
	{

	}

	SpriteRenderer::SpriteRenderer(Ref<Texture2D>& texture)
		: m_Texture(texture)
	{

	}

	void SpriteRenderer::OnUpdate(Timestep ts)
	{
		Renderer2D::DrawQuad({ 0.25f, -0.25f , -2.0f }, { .2f, .2f }, {1.0f, 0.0f, 1.0f, 1.0f});
	}
}