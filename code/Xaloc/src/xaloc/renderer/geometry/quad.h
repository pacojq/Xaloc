#pragma once
#include "xaloc/types.h"
#include <glm/glm.hpp>


namespace Xaloc
{

struct SQuadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

class ZQuadBuilder
{
public:
	ZQuadBuilder();

	void Reset();

	SQuadVertex Get(uint32 nVertex) const;

	void SetTransform(glm::mat4 transform);

	void SetTexcoords(const glm::vec2* aTexcoords);
	void SetTexcoord(glm::vec2 texcoord, uint32 nVertex);

	void SetColor(glm::vec4 color);
	void SetColor(glm::vec4 color, uint32 nVertex);

	void SetTextureIndex(float32 fTextureIndex);

	void SetTilingFactor(float32 fTilingFactor);

private:
	SQuadVertex m_aQuads[4];
};

}