#pragma once

#include "Xaloc.h"


class PlayerComponent : public Xaloc::Component
{
public:
	void OnUpdate(Xaloc::Timestep ts) override;

	void SetSpeed(float spd) { m_Speed = spd; }
	float GetSpeed() const { return m_Speed; }

private:
	float m_Speed = 2.0f;
};