#include "xapch.h"
#include "xaloc/core/input/input.h"
#include "xaloc/core/application.h"
#include <GLFW/glfw3.h>


namespace Xaloc
{

static inline GLFWwindow* GetWindow()
{
	return static_cast<GLFWwindow*>(ZApplication::Get()->GetWindow().GetNativeWindow());
}

// TODO handle non-QWERTY keyboards with GLFW

bool ZInput::IsKeyPressed(EKeyCode eKey)
{
	GLFWwindow* pWindow = GetWindow();
	auto state = glfwGetKey(pWindow, static_cast<int32_t>(eKey));
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool ZInput::IsGamepadButtonPressed(int32 nGamepad, int32 nButton)
{
	GLFWgamepadstate gpState;
	if (glfwGetGamepadState(nGamepad, &gpState))
	{
		auto state = gpState.buttons[nButton];
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	return false;
}

bool ZInput::IsGamepadConnected(int32 nGamepad)
{
	return glfwJoystickPresent(nGamepad) && glfwJoystickIsGamepad(nGamepad);
}

bool ZInput::IsMouseButtonPressed(int32 nButton)
{
	GLFWwindow* pWindow = GetWindow();
	auto state = glfwGetMouseButton(pWindow, nButton);
	return state == GLFW_PRESS;
}

std::pair<float, float>  ZInput::GetMousePosition()
{
	GLFWwindow* pWindow = GetWindow();
	double fPosX, fPosY;
	glfwGetCursorPos(pWindow, &fPosX, &fPosY);
	return { (float)fPosX, (float)fPosY };
}

float ZInput::GetMouseX()
{
	auto[x, y] = GetMousePosition();
	return x;
}

float ZInput::GetMouseY()
{
	auto[x, y] = GetMousePosition();
	return y;
}

}