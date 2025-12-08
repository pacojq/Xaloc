#include "xapch.h"
#include "xaloc/renderer/geometry/quad.h"
#include <glm/gtc/matrix_transform.hpp>


namespace Xaloc
{

// Positions, in UV coords, of each vertex in a quad
static glm::vec4 s_aQuadVertexPositions[4] =
{
	{ -0.5f, -0.5f, 0.0f, 1.0f }, // Bottom left
	{ 0.5f, -0.5f, 0.0f, 1.0f },  // Bottom right
	{ 0.5f,  0.5f, 0.0f, 1.0f },  // Top right
	{ -0.5f,  0.5f, 0.0f, 1.0f }  // Top left
};

ZQuadBuilder::ZQuadBuilder()
{
	Reset();
}

void ZQuadBuilder::Reset()
{
	const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	const glm::vec2 textureCoords[4] =
	{
		{ 0.0f, 0.0f }, // Bottom left
		{ 1.0f, 0.0f }, // Bottom right
		{ 1.0f, 1.0f }, // Top right
		{ 0.0f, 1.0f }  // Top left
	};

	for (uint32 nQuad = 0; nQuad < 4; ++nQuad)
	{
		SQuadVertex& rQuad = m_aQuads[nQuad];
		rQuad.Position = s_aQuadVertexPositions[nQuad];
		rQuad.Color = color;
		rQuad.TexCoord = textureCoords[nQuad];
		rQuad.TexIndex = 0.0f; // White texture
		rQuad.TilingFactor = 1.0f;
	}
}

SQuadVertex ZQuadBuilder::Get(uint32 nVertex) const
{
	return m_aQuads[nVertex];
}

void ZQuadBuilder::SetTransform(
	glm::mat4 transform)
{
	for (uint32 nQuad = 0; nQuad < 4; ++nQuad)
	{
		SQuadVertex& rQuad = m_aQuads[nQuad];
		rQuad.Position = transform * s_aQuadVertexPositions[nQuad];
	}
}

void ZQuadBuilder::SetTexcoords(
	const glm::vec2* aTexcoords)
{
	for (uint32 nQuad = 0; nQuad < 4; ++nQuad)
	{
		SQuadVertex& rQuad = m_aQuads[nQuad];
		rQuad.TexCoord = aTexcoords[nQuad];
	}
}

void ZQuadBuilder::SetTexcoord(
	glm::vec2 texcoord,
	uint32 nVertex)
{
	SQuadVertex& rQuad = m_aQuads[nVertex];
	rQuad.TexCoord = texcoord;
}

void ZQuadBuilder::SetColor(
	glm::vec4 color)
{
	for (uint32 nQuad = 0; nQuad < 4; ++nQuad)
	{
		SQuadVertex& rQuad = m_aQuads[nQuad];
		rQuad.Color = color;
	}
}

void ZQuadBuilder::SetColor(
	glm::vec4 color,
	uint32 nVertex)
{
	SQuadVertex& rQuad = m_aQuads[nVertex];
	rQuad.Color = color;
}

void ZQuadBuilder::SetTextureIndex(
	float32 fTextureIndex)
{
	for (uint32 nQuad = 0; nQuad < 4; ++nQuad)
	{
		SQuadVertex& rQuad = m_aQuads[nQuad];
		rQuad.TexIndex = fTextureIndex;
	}
}

void ZQuadBuilder::SetTilingFactor(
	float32 fTilingFactor)
{
	for (uint32 nQuad = 0; nQuad < 4; ++nQuad)
	{
		SQuadVertex& rQuad = m_aQuads[nQuad];
		rQuad.TilingFactor = fTilingFactor;
	}
}

}