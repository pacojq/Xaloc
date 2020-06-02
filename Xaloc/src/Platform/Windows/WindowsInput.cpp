#include "xapch.h"
#include "WindowsInput.h"

#include <GLFW/glfw3.h>
#include "Xaloc/Core/Application.h"

namespace Xaloc {

	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();


	bool WindowsInput::IsKeyPressedImpl(int keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsGamepadButtonPressedImpl(int id, int button)
	{
		GLFWgamepadstate gpState;

		if (glfwGetGamepadState(id, &gpState))
		{
			auto state = gpState.buttons[button];
			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}
		
		return false;
	}

	bool WindowsInput::IsGamepadConnectedImpl(int id)
	{
		return glfwJoystickPresent(id) && glfwJoystickIsGamepad(id);
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float>  WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return y;
	}
}