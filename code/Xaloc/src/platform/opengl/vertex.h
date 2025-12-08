#pragma once
#include "xaloc/renderer/geometry/vertex.h"


namespace Xaloc
{

class ZVertexBufferOpenGL : public ZVertexBuffer
{
public:
	ZVertexBufferOpenGL(uint32 nSize);
	ZVertexBufferOpenGL(float32* aVertices, uint32 nSize);
	virtual ~ZVertexBufferOpenGL();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void SetData(const void* pData, uint32 nSize) override;

	virtual const ZVertexBufferLayout& GetLayout() const override { return m_Layout; }
	virtual void SetLayout(const ZVertexBufferLayout& layout) override { m_Layout = layout; }

private:
	uint32 m_idRenderer;
	ZVertexBufferLayout m_Layout;
};

}