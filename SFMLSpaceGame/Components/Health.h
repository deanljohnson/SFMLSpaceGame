#pragma once
#include "Component.h"
#include <Entity.h>

class Health : public Component
{
private:
	float m_maxHealth{ 100.f };
	float m_health{ 100.f };
public:

	inline float GetHealth() const { return m_health; }
	inline void SetHealth(float hp)
	{
		m_health = std::max(0.f, hp);
		if (m_health == 0.f)
			entity->Destroy();
	}
	inline void RemoveHealth(float amt) { SetHealth(m_health - amt); }
	inline float GetHealthNormalized() { return m_health / m_maxHealth; }
};