#include "PlayerComponent.h"

void PlayerComponent::OnUpdate(Xaloc::Timestep ts)
{
	glm::vec3 pos = GetGameObject()->GetPosition();
	
	if (Xaloc::Input::IsKeyPressed(XA_KEY_LEFT))
		pos.x -= m_Speed * ts;
	if (Xaloc::Input::IsKeyPressed(XA_KEY_RIGHT))
		pos.x += m_Speed * ts;
	if (Xaloc::Input::IsKeyPressed(XA_KEY_UP))
		pos.y += m_Speed * ts;
	if (Xaloc::Input::IsKeyPressed(XA_KEY_DOWN))
		pos.y -= m_Speed * ts;

	GetGameObject()->SetPosition(pos);
}