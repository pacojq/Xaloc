#include "xapch.h"
#include "platform/windows/window.h"
#include "platform/opengl/context.h"
#include "xaloc/events/application.h"
#include "xaloc/events/mouse.h"
#include "xaloc/events/key.h"
#include "xaloc/debugging/assert.h"


namespace Xaloc
{

static bool s_GLFWInitialized = false;

float ZWindow::s_HighDPIScaleFactor = 1.0f;
	
static void GLFWErrorCallback(int error, const char* description)
{
	XA_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

static SWindowsWindow* GetWindow(GLFWwindow* pGlfwWindow)
{
	return (SWindowsWindow*)glfwGetWindowUserPointer(pGlfwWindow);
}

/// <summary>
/// TODO:
/// 
/// ACCORDING TO GLFW DOCS:
///		The window size is in screen coordinates, not pixels.
///		Use the framebuffer size, which is in pixels, for pixel-based calls.
/// 
///		 glfwSetFramebufferSizeCallback(
///			window,
///			framebuffer_size_callback);
///
///		void framebuffer_size_callback(
///			GLFWwindow * window,
///			int width,
///			int height)
///		{
///			glViewport(0, 0, width, height);
///		}
///
///		int width, height;
///		glfwGetFramebufferSize(window, &width, &height);
///		glViewport(0, 0, width, height);
///
/// </summary>
static void GlfwCallbackWindowSize(
	GLFWwindow* pGlfwWindow,
	int nWidth,
	int nHeight)
{
	// TODO: handle sizes < 0 ??? check GLFW documentation.

	SWindowsWindow* pWindow = GetWindow(pGlfwWindow);
	pWindow->nWidth = uint32(nWidth);
	pWindow->nHeight = uint32(nHeight);

	EvtWindowResize event(pWindow->nWidth, pWindow->nHeight);
	pWindow->EventCallback(event);
}

static void GlfwCallbackWindowClose(GLFWwindow* pGlfwWindow)
{
	SWindowsWindow* pWindow = GetWindow(pGlfwWindow);
	EvtWindowClose event;
	pWindow->EventCallback(event);
}

static void GlfwCallbackWindowFocus(GLFWwindow* pGlfwWindow, int bFocused)
{
	SWindowsWindow* pWindow = GetWindow(pGlfwWindow);
	EvtWindowFocus event(bFocused);
	pWindow->EventCallback(event);
}

static void GlfwCallbackKey(
	GLFWwindow* pGlfwWindow,
	int eKey,
	int nScancode,
	int eAction,
	int nMods)
{
	SWindowsWindow* pWindow = GetWindow(pGlfwWindow);
	switch (eAction)
	{
	case GLFW_PRESS:
	{
		EvtKeyPressed event(eKey, 0);
		pWindow->EventCallback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		EvtKeyReleased event(eKey);
		pWindow->EventCallback(event);
		break;
	}
	case GLFW_REPEAT:
	{
		EvtKeyPressed event(eKey, 1);
		pWindow->EventCallback(event);
		break;
	}
	}
}

static void GlfwCallbackChar(GLFWwindow* pGlfwWindow, unsigned int nCharacter)
{
	SWindowsWindow* pWindow = GetWindow(pGlfwWindow);
	EvtKeyTyped event(nCharacter);
	pWindow->EventCallback(event);
}

static void GlfwCallbackJoystick(int idJoystick, int ev)
{
	if (ev == GLFW_CONNECTED)
	{
		if (glfwJoystickIsGamepad(idJoystick))
		{
			XA_CORE_INFO("Gamepad connected [id={0}] {1} - {2}",
				idJoystick,
				glfwGetJoystickName(idJoystick),
				glfwGetGamepadName(idJoystick));
		}
		else
		{
			XA_CORE_INFO("Joystick connected [id={0}] {1}",
				idJoystick,
				glfwGetJoystickName(idJoystick));
		}
	}
	else if (ev == GLFW_DISCONNECTED)
	{
		XA_CORE_INFO("Joystick disconnected [id={0}]",
			idJoystick);
	}
}

static void GlfwCallbackMouseButton(GLFWwindow* pGlfwWindow, int button, int action, int mods)
{
	SWindowsWindow* pWindow = GetWindow(pGlfwWindow);
	switch (action)
	{
	case GLFW_PRESS:
	{
		EvtMouseButtonPressed event(button);
		pWindow->EventCallback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		EvtMouseButtonReleased event(button);
		pWindow->EventCallback(event);
		break;
	}
	}
}

static void GlfwCallbackScroll(GLFWwindow* pGlfwWindow, double xOffset, double yOffset)
{
	SWindowsWindow* pWindow = GetWindow(pGlfwWindow);
	EvtMouseScrolled event((float)xOffset, (float)yOffset);
	pWindow->EventCallback(event);
}

static void GlfwCallbackCursorPos(GLFWwindow* pGlfwWindow, double xPos, double yPos)
{
	SWindowsWindow* pWindow = GetWindow(pGlfwWindow);
	EvtMouseMoved event((float)xPos, (float)yPos);
	pWindow->EventCallback(event);
}

ZWindow* ZWindow::Create(
	ERenderAPI eRenderAPI,
	const SWindowDefinition& definition)
{
	return new ZWindowsWindow(eRenderAPI, definition);
}

ZWindowsWindow::ZWindowsWindow(
	ERenderAPI eRenderAPI,
	const SWindowDefinition& definition)
{
	Init(eRenderAPI, definition);
}

ZWindowsWindow::~ZWindowsWindow()
{
	Shutdown();
}

void ZWindowsWindow::Init(
	ERenderAPI eRenderAPI,
	const SWindowDefinition& definition)
{
	m_Data.sTitle = definition.sTitle;
	m_Data.nWidth = definition.nWidth;
	m_Data.nHeight = definition.nHeight;

	if (!s_GLFWInitialized)
	{
		// TODO: glfwTerminate on system shutdown
		int success = glfwInit();
		XA_CORE_ASSERT(success, "Could not initialize GLFW!");
		glfwSetErrorCallback(GLFWErrorCallback);
		s_GLFWInitialized = true;
	}

	//int videoModeCount;
	//const GLFWvidmode* modes = glfwGetVideoModes(primary, &videoModeCount);
	//XA_CORE_INFO("Primary monitor:");
	//for (int i = 0; i < videoModeCount; i++)
	//{
	//	XA_CORE_INFO("    Video mode [{0}]: {1} x {2} | {3} Hz", i, modes[i].width, modes[i].height, modes[i].refreshRate);
	//}
	//

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primary);
	XA_CORE_INFO("Current video mode: {0} x {1} | {2} Hz", mode->width, mode->height, mode->refreshRate);
	XA_CORE_INFO("Creating window '{0}' ({1}, {2})", definition.sTitle, definition.nWidth, definition.nHeight);

	float xscale, yscale;
	glfwGetMonitorContentScale(primary, &xscale, &yscale);
	if (xscale > 1.0f || yscale > 1.0f)
	{
		ZWindow::s_HighDPIScaleFactor = xscale;
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	}

	glfwWindowHint(GLFW_RESIZABLE, definition.bResizable ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, definition.bDecorated ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_REFRESH_RATE, 0); // Run at max refresh rate

	m_Window = glfwCreateWindow(
		(int)definition.nWidth,
		(int)definition.nHeight,
		m_Data.sTitle.c_str(),
		nullptr,
		nullptr);

	if (eRenderAPI == ERenderAPI::OpenGL)
	{
		m_pContext = new ZGraphicsContextOpenGL(m_Window);
	}
	else
	{
		XA_CORE_ASSERT(false, "Unknown render API!");
	}

	m_pContext->Init();

	glfwSetWindowUserPointer(m_Window, &m_Data);
	SetVSync(definition.bVSyncEnabled);

	// Gamepad discovery
	for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
	{
		if (glfwJoystickPresent(i))
		{
			if (glfwJoystickIsGamepad(i))
				XA_CORE_INFO("Gamepad detected [id={0}] {1} - {2}", i, glfwGetJoystickName(i), glfwGetGamepadName(i));
			else XA_CORE_INFO("Joystick detected [id={0}] {1}", i, glfwGetJoystickName(i));
		}
	}

	glfwSetWindowSizeCallback(m_Window,  GlfwCallbackWindowSize);
	glfwSetWindowCloseCallback(m_Window, GlfwCallbackWindowClose);
	glfwSetWindowFocusCallback(m_Window, GlfwCallbackWindowFocus);
	glfwSetKeyCallback(m_Window,         GlfwCallbackKey);
	glfwSetCharCallback(m_Window,        GlfwCallbackChar);
	glfwSetJoystickCallback(             GlfwCallbackJoystick);
	glfwSetMouseButtonCallback(m_Window, GlfwCallbackMouseButton);
	glfwSetScrollCallback(m_Window,      GlfwCallbackScroll);
	glfwSetCursorPosCallback(m_Window,   GlfwCallbackCursorPos);
}

void ZWindowsWindow::Shutdown()
{
	glfwDestroyWindow(m_Window);
	delete m_pContext;
}

void ZWindowsWindow::OnUpdate()
{
	glfwPollEvents();
	m_pContext->SwapBuffers();
}

void ZWindowsWindow::SetVSync(bool enabled)
{
	glfwSwapInterval(enabled ? 1 : 0);
	m_Data.bVSync = enabled;
}

bool ZWindowsWindow::IsVSync() const
{
	return m_Data.bVSync;
}

}
