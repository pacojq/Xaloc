#pragma once

#include "Xaloc/Core/Input/Input.h"

namespace Xaloc {

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keyCode) override;
		
		virtual bool IsGamepadButtonPressedImpl(int id, int button) override;
		virtual bool IsGamepadConnectedImpl(int id) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}