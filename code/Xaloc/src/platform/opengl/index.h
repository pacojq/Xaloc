#pragma once
#include "xaloc/renderer/geometry/index.h"


namespace Xaloc
{

class ZIndexBufferOpenGL : public ZIndexBuffer
{
public:
	ZIndexBufferOpenGL(uint32* aIndices, uint32 nCount);
	virtual ~ZIndexBufferOpenGL();

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual uint32 GetCount() const { return m_nCount; }

private:
	uint32 m_idRenderer;
	uint32 m_nCount;
};

}