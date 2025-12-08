#pragma once
#include "xaloc/renderer/resource/texture.h"
#include <glm/glm.hpp>


namespace Xaloc
{

class ZSubTexture2D : public Asset
{
public:
	ZSubTexture2D(
		const ZRef<ZTexture2D>& texture,
		const glm::vec2& min,
		const glm::vec2& max);

	const ZRef<ZTexture2D>& GetTexture() const { return m_pTexture; }
	const glm::vec2* GetTexCoords() const { return m_aTexCoords; }

	uint32 GetWidth() const { return m_nWidth; }
	uint32 GetHeight() const { return m_nHeight; }

	static ZRef<ZSubTexture2D> CreateFromGrid(
		const ZRef<ZTexture2D>& texture,
		const glm::vec2& cellCoords,
		const glm::vec2& cellSize,
		const glm::vec2& padding = { 0.0f, 0.0f },
		const glm::vec2& offset = { 0.0f, 0.0f });

	static ZRef<ZSubTexture2D> CreateFromAbsCoords(
		const ZRef<ZTexture2D>& texture,
		const glm::vec2& absCoords, 
		const glm::vec2& spriteSize);

	static EAssetType GetStaticAssetType() { return EAssetType::Sprite; }
	virtual EAssetType GetAssetType() const override { return GetStaticAssetType(); }

private:
	uint32 m_nWidth;
	uint32 m_nHeight;
	glm::vec2 m_aTexCoords[4];
	ZRef<ZTexture2D> m_pTexture;
};

}