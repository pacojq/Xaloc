#pragma once

namespace Xaloc
{

class ZGraphicsContext
{
public:
	virtual ~ZGraphicsContext() = default;

	virtual void Init() = 0;
	virtual void SwapBuffers() = 0;
};

}