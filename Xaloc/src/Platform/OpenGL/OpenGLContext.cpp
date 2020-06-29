#include "xapch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Xaloc {

	
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		XA_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		XA_CORE_TRACE("OpenGL Context initializing...");
		
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		XA_CORE_ASSERT(status, "Failed to initialize Glad!");

		XA_CORE_INFO("OpenGL Renderer:");
		XA_CORE_INFO("  Vendor:    {0}", glGetString(GL_VENDOR));
		XA_CORE_INFO("  Renderer:  {0}", glGetString(GL_RENDERER));
		XA_CORE_INFO("  Version:   {0}", glGetString(GL_VERSION));

#ifdef XA_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		XA_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Xaloc requires at least OpenGL version 4.5!");
#endif

		XA_CORE_TRACE("OpenGL Context ready!");
	}


	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}