#include "xapch.h"
#include "SubTexture2D.h"

namespace Xaloc {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}


	Ref<SubTexture2D> SubTexture2D::CreateFromGrid(const Ref<Texture2D>& texture, const glm::vec2& cellCoords, 
		const glm::vec2& cellSize, const glm::vec2& padding, const glm::vec2& offset)
	{
		glm::vec2 min = {
			(padding.x + cellCoords.x * (cellSize.x + offset.x)) / texture->GetWidth(),
			(padding.y + cellCoords.y * (cellSize.y + offset.y)) / texture->GetHeight()
		};

		glm::vec2 max = {
			min.x + (cellSize.x / texture->GetWidth()),
			min.y + (cellSize.y / texture->GetHeight())
		};

		return CreateRef<SubTexture2D>(texture, min, max);
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromAbsCoords(const Ref<Texture2D>& texture, const glm::vec2& absCoords,
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

		return CreateRef<SubTexture2D>(texture, min, max);
	}

}