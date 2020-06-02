#include "PlayerComponent.h"

void PlayerComponent::OnUpdate(Xaloc::Timestep ts)
{
	glm::vec3 pos = GetGameObject()->GetPosition();
	
	if (Xaloc::Input::IsKeyPressed(XA_KEY_LEFT) || Xaloc::Input::IsGamepadButtonPressed(0, XA_GAMEPAD_BUTTON_DPAD_LEFT))
		pos.x -= m_Speed * ts;
	if (Xaloc::Input::IsKeyPressed(XA_KEY_RIGHT) || Xaloc::Input::IsGamepadButtonPressed(0, XA_GAMEPAD_BUTTON_DPAD_RIGHT))
		pos.x += m_Speed * ts;
	if (Xaloc::Input::IsKeyPressed(XA_KEY_UP) || Xaloc::Input::IsGamepadButtonPressed(0, XA_GAMEPAD_BUTTON_DPAD_UP))
		pos.y += m_Speed * ts;
	if (Xaloc::Input::IsKeyPressed(XA_KEY_DOWN) || Xaloc::Input::IsGamepadButtonPressed(0, XA_GAMEPAD_BUTTON_DPAD_DOWN))
		pos.y -= m_Speed * ts;

	GetGameObject()->SetPosition(pos);
}