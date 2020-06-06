#include "xapch.h"
#include "SpriteRenderer.h"

#include "Xaloc/Renderer/Renderer2D.h"

namespace Xaloc {

	//SpriteRenderer::SpriteRenderer(const std::string& filename)
	//{
	//	// TODO
	//}

	SpriteRenderer::SpriteRenderer(const Ref<Texture2D>& texture)
	{
		glm::vec2 min = { 0.0f, 0.0f };
		glm::vec2 max = { 1.0f, 1.0f };
		m_SubTexture = CreateRef<SubTexture2D>(texture, min, max);
	}

	SpriteRenderer::SpriteRenderer(const Ref<SubTexture2D>& subTexture)
		: m_SubTexture(subTexture)
	{
	}

	//void SpriteRenderer::OnUpdate(Timestep ts)
	//{
	//	glm::vec3 pos = GetPosition();
	//	Renderer2D::DrawQuad({ pos.x, pos.y, m_Depth }, { 1.0f, 1.0f }, m_SubTexture);
	//}
}