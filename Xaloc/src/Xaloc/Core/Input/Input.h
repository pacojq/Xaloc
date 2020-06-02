#pragma once

#include "Xaloc/Core/Core.h"

namespace Xaloc {

	class XALOC_API Input
	{

	public:
		inline static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }

		inline static bool IsGamepadButtonPressed(int id, int button) { return s_Instance->IsGamepadButtonPressedImpl(id, button); }
		inline static bool IsGamepadConnected(int id) { return s_Instance->IsGamepadConnectedImpl(id); }

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }


	protected:
		virtual bool IsKeyPressedImpl(int keyCode) = 0;
		
		virtual bool IsGamepadButtonPressedImpl(int id, int button) = 0;
		virtual bool IsGamepadConnectedImpl(int id) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Scope<Input> s_Instance;
	};

}