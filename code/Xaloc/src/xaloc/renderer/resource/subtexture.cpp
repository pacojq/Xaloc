#include "xapch.h"
#include "xaloc/renderer/resource/subtexture.h"


namespace Xaloc
{

ZSubTexture2D::ZSubTexture2D(
	const ZRef<ZTexture2D>& texture,
	const glm::vec2& min,
	const glm::vec2& max)
	: m_pTexture(texture)
{
	m_aTexCoords[0] = { min.x, min.y };
	m_aTexCoords[1] = { max.x, min.y };
	m_aTexCoords[2] = { max.x, max.y };
	m_aTexCoords[3] = { min.x, max.y };

	m_nWidth = (int)glm::round((m_aTexCoords[2].x - m_aTexCoords[0].x) * texture->GetWidth());
	m_nHeight = (int)glm::round((m_aTexCoords[2].y - m_aTexCoords[0].y) * texture->GetHeight());
}

ZRef<ZSubTexture2D> ZSubTexture2D::CreateFromGrid(
	const ZRef<ZTexture2D>& texture,
	const glm::vec2& cellCoords,
	const glm::vec2& cellSize,
	const glm::vec2& padding,
	const glm::vec2& offset)
{
	glm::vec2 min = {
		(padding.x + cellCoords.x * (cellSize.x + offset.x)) / texture->GetWidth(),
		(padding.y + cellCoords.y * (cellSize.y + offset.y)) / texture->GetHeight()
	};

	glm::vec2 max = {
		min.x + (cellSize.x / texture->GetWidth()),
		min.y + (cellSize.y / texture->GetHeight())
	};

	return CreateRef<ZSubTexture2D>(texture, min, max);
}

ZRef<ZSubTexture2D> ZSubTexture2D::CreateFromAbsCoords(
	const ZRef<ZTexture2D>& texture,
	const glm::vec2& absCoords,
	const glm::vec2& spriteSize)
{
	glm::vec2 min = {
		absCoords.x / texture->GetWidth(),
		absCoords.y / texture->GetHeight()
	};

	glm::vec2 max = {
		min.x + (spriteSize.x / texture->GetWidth()),
		min.y + (spriteSize.y / texture->GetHeight())
	};

	return CreateRef<ZSubTexture2D>(texture, min, max);
}

}