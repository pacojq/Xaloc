#include "xapch.h"
#include "Xaloc/Core/Input/Input.h"

#include <GLFW/glfw3.h>
#include "Xaloc/Core/Application.h"

namespace Xaloc {

	// TODO handle non-QWERTY keyboards with GLFW


	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(keyCode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsGamepadButtonPressed(int id, int button)
	{
		GLFWgamepadstate gpState;

		if (glfwGetGamepadState(id, &gpState))
		{
			auto state = gpState.buttons[button];
			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}
		
		return false;
	}

	bool Input::IsGamepadConnected(int id)
	{
		return glfwJoystickPresent(id) && glfwJoystickIsGamepad(id);
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float>  Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
	}

	float Input::GetMouseX()
	{
		auto[x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto[x, y] = GetMousePosition();
		return y;
	}
}