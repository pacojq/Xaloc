#include "xapch.h"
#include "WindowsDirectXWindow.h"

#include "Xaloc/Core/Input/MouseButtonCodes.h"

#include "Xaloc/Events/ApplicationEvent.h"
#include "Xaloc/Events/MouseEvent.h"
#include "Xaloc/Events/KeyEvent.h"

#include "Xaloc/Renderer/Renderer.h"
#include "Platform/DirectX/DirectXContext.h"

//#include <examples/imgui_impl_win32.h>
#include "examples/imgui_impl_dx11.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include <system_error>

namespace Xaloc {
	
	WindowsDirectXWindow::WindowsDirectXWindow(const WindowProps& props)
	{
		m_ClassName = L"XalocWindowClass";

		Init(props);
	}

	WindowsDirectXWindow::~WindowsDirectXWindow()
	{
		Shutdown();
	}

	void WindowsDirectXWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		XA_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);


		WNDCLASS wc = {};
		wc.lpfnWndProc = StaticWindowProc;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpszClassName = m_ClassName;
		RegisterClass(&wc);

		wchar_t title[32];
		mbstowcs_s(nullptr, title, m_Data.Title.c_str(), std::size(title));

		RECT wr;
		wr.left = 100;
		wr.right = m_Data.Width + wr.left;
		wr.top = 100;
		wr.bottom = m_Data.Height + wr.top;
		AdjustWindowRectEx(&wr, WS_OVERLAPPEDWINDOW, FALSE, 0);

		m_Handle = CreateWindowEx(0, m_ClassName,
			title,
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, GetModuleHandle(nullptr),
			this);

		UnregisterClass(m_ClassName, GetModuleHandle(nullptr));

		XA_CORE_ASSERT(m_Handle, std::system_category().message(GetLastError()));

		ShowWindow(m_Handle, SW_SHOWDEFAULT);

		// DirectX Context
		m_Context = CreateScope<DirectXContext>(m_Handle, m_Data.Width, m_Data.Height);
		m_Context->Init();
	}

	void WindowsDirectXWindow::Shutdown()
	{
		XA_CORE_INFO("Destroying DirectX window");
		DestroyWindow(m_Handle);
	}

	void WindowsDirectXWindow::OnUpdate()
	{
		// TODO glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsDirectXWindow::SetVSync(bool enabled)
	{
		// TODO

		m_Data.VSync = enabled;
	}

	bool WindowsDirectXWindow::IsVSync() const
	{
		return m_Data.VSync;
	}







	LRESULT WindowsDirectXWindow::StaticWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		WindowsDirectXWindow* instance = nullptr;

		if (msg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lparam;
			instance = (WindowsDirectXWindow*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)instance);

			instance->m_Handle = hwnd;
		}
		else
		{
			instance = (WindowsDirectXWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}

		if (instance)
		{
			return instance->MessageCallback(msg, wparam, lparam);
		}
		else return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	LRESULT WindowsDirectXWindow::MessageCallback(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (ImGui_ImplWin32_WndProcHandler(m_Handle, msg, wparam, lparam))
			return true;

		switch (msg)
		{
			// Application Events
			case WM_CLOSE:
			{
				WindowCloseEvent event;
				m_Data.EventCallback(event);
				break;
			}
			case WM_SIZE:
			{
				m_Data.Width = LOWORD(lparam);
				m_Data.Height = HIWORD(lparam);
				
				WindowResizeEvent event(m_Data.Width, m_Data.Width);
				if (m_Data.EventCallback) m_Data.EventCallback(event);
				break;
			}

			// Keyboard events
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				int repeats = lparam & 0xffff;
				KeyPressedEvent event((int)wparam, repeats);
				m_Data.EventCallback(event);
				break;
			}
			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				KeyReleasedEvent event((int)wparam);
				m_Data.EventCallback(event);
				break;
			}

			// Mouse events
			case WM_MOUSEMOVE:
			{
				auto pos = MAKEPOINTS(lparam);
				MouseMovedEvent event(pos.x, pos.y);
				m_Data.EventCallback(event);
				break;
			}
			case WM_LBUTTONDOWN:
			{
				MouseButtonPressedEvent event(XA_MOUSE_BUTTON_LEFT);
				m_Data.EventCallback(event);
				break;
			}
			case WM_LBUTTONUP:
			{
				MouseButtonReleasedEvent event(XA_MOUSE_BUTTON_LEFT);
				m_Data.EventCallback(event);
				break;
			}
			case WM_RBUTTONDOWN:
			{
				MouseButtonPressedEvent event(XA_MOUSE_BUTTON_RIGHT);
				m_Data.EventCallback(event);
				break;
			}
			case WM_RBUTTONUP:
			{
				MouseButtonReleasedEvent event(XA_MOUSE_BUTTON_RIGHT);
				m_Data.EventCallback(event);
				break;
			}
			case WM_MBUTTONDOWN:
			{
				MouseButtonPressedEvent event(XA_MOUSE_BUTTON_MIDDLE);
				m_Data.EventCallback(event);
				break;
			}
			case WM_MBUTTONUP:
			{
				MouseButtonReleasedEvent event(XA_MOUSE_BUTTON_MIDDLE);
				m_Data.EventCallback(event);
				break;
			}
			case WM_MOUSEWHEEL:
			{
				static int accum = 0;
				accum += GET_WHEEL_DELTA_WPARAM(wparam);
				while (accum >= WHEEL_DELTA)
				{
					accum -= WHEEL_DELTA;
					MouseScrolledEvent event(0, 1);
					m_Data.EventCallback(event);
				}
				while (accum <= -WHEEL_DELTA)
				{
					accum += WHEEL_DELTA;
					MouseScrolledEvent event(0, -1);
					m_Data.EventCallback(event);
				}
				break;
			}
		}

		return DefWindowProc(m_Handle, msg, wparam, lparam);
	}

}
