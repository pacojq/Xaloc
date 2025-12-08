#pragma once
#include "xaloc/renderer/context.h"

struct GLFWwindow;


namespace Xaloc
{

class ZGraphicsContextOpenGL : public ZGraphicsContext
{
public:
	ZGraphicsContextOpenGL(GLFWwindow* windowHandle);

	virtual void Init() override;
	virtual void SwapBuffers() override;

private:
	GLFWwindow* m_WindowHandle;
};

}