#pragma once
#include "xaloc/types.h"


namespace Xaloc
{

class ZIndexBuffer
{
public:
	virtual ~ZIndexBuffer() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual uint32 GetCount() const = 0;

	static ZRef<ZIndexBuffer> Create(
		uint32* aIndeces,
		uint32 nCount);
};

}