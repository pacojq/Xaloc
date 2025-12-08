#pragma once
#include "xaloc/renderer/api.h"


namespace Xaloc
{

class ZRendererAPIOpenGL : public ZRendererAPI
{
public:
	virtual void Init() override;
	virtual void SetViewport(
		uint32 nX,
		uint32 nY,
		uint32 nWidth,
		uint32 nHeight) override;

	virtual void SetClearColor(const glm::vec4& color) override;
	virtual void Clear() override;

	virtual void DrawIndexed(uint32 nIndexCount = 0) override;

	virtual void DrawBlit() override;
};

}