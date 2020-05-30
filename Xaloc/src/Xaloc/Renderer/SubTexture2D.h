#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

namespace Xaloc {

	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }


		static Ref<SubTexture2D> CreateFromGrid(const Ref<Texture2D>& texture, const glm::vec2& cellCoords, 
			const glm::vec2& cellSize, const glm::vec2& padding = { 0.0f, 0.0f }, const glm::vec2& offset = { 0.0f, 0.0f });

		static Ref<SubTexture2D> CreateFromAbsCoords(const Ref<Texture2D>& texture, const glm::vec2& absCoords, 
			const glm::vec2& spriteSize);

	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};
}