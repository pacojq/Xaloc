#include "xapch.h"
#include "WindowsWindow.h"

#include "Xaloc/Events/ApplicationEvent.h"
#include "Xaloc/Events/MouseEvent.h"
#include "Xaloc/Events/KeyEvent.h"

#include "Xaloc/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/Vulkan/VulkanContext.h"


namespace Xaloc {

	static bool s_GLFWInitialized = false;

	float Window::s_HighDPIScaleFactor = 1.0f;
	
	static void GLFWErrorCallback(int error, const char* description)
	{
		XA_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}


	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;



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
		
		XA_CORE_INFO("Creating window '{0}' ({1}, {2})", props.Title, props.Width, props.Height);

		float xscale, yscale;
		glfwGetMonitorContentScale(primary, &xscale, &yscale);
		if (xscale > 1.0f || yscale > 1.0f)
		{
			Window::s_HighDPIScaleFactor = xscale;
			glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
		}
		
		glfwWindowHint(GLFW_RESIZABLE, props.IsResizable ? GLFW_TRUE : GLFW_FALSE);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
		glfwWindowHint(GLFW_DECORATED, props.IsDecorated ? GLFW_TRUE : GLFW_FALSE);
		glfwWindowHint(GLFW_REFRESH_RATE, 0); // Run at max refresh rate

		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);


		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			m_Context = CreateScope<OpenGLContext>(m_Window);
		else if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
			m_Context = CreateScope<VulkanContext>(m_Window);
		
		//m_Context = CreateScope<OpenGLContext>(m_Window);
		m_Context->Init();
		
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(props.IsVSyncEnabled);


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

		
		


		
		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);			
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowFocusEvent event(focused);
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
			
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(character);
			data.EventCallback(event);
		});


		glfwSetJoystickCallback([](int joystickId, int ev)
		{
			if (ev == GLFW_CONNECTED)
			{
				if (glfwJoystickIsGamepad(joystickId))
				{
					XA_CORE_INFO("Gamepad connected [id={0}] {1} - {2}", joystickId, glfwGetJoystickName(joystickId), glfwGetGamepadName(joystickId));
				}
				else
				{
					XA_CORE_INFO("Joystick connected [id={0}] {1}", joystickId, glfwGetJoystickName(joystickId));
				}
			}
			else if (ev == GLFW_DISCONNECTED)
			{
				XA_CORE_INFO("Joystick disconnected [id={0}]", joystickId);
			}
		});


		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
