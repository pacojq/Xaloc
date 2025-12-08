#pragma once
#include "xaloc/types.h"
#include "xaloc/core/core.h"
#include "xaloc/core/input/keycode.h"


namespace Xaloc
{

class ZInput
{
public:
	static bool IsKeyPressed(EKeyCode eKey);

	static bool IsGamepadButtonPressed(int32 nGamepad, int32 nButton);
	static bool IsGamepadConnected(int32 nGamepad);

	static bool IsMouseButtonPressed(int32 nButton);
	static std::pair<float32, float32> GetMousePosition();
	static float32 GetMouseX();
	static float32 GetMouseY();
};

}