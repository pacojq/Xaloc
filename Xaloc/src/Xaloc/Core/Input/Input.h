#pragma once

#include "Xaloc/Core/Core.h"
#include "KeyCodes.h"

namespace Xaloc {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keyCode);

		static bool IsGamepadButtonPressed(int id, int button);
		static bool IsGamepadConnected(int id);

		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}