#pragma once
#include "Health.h"
#include <Interfaces\AttackEventModifier.h>

class DamageOnAttacked : public Component
{
private:
	Health* m_health;
	std::vector<AttackedEventModifier*> m_modifiers;

	void HandleAttack(Event& event);
public:
	DamageOnAttacked() 
		: m_health(nullptr),
		  m_modifiers()
	{}
	DamageOnAttacked(std::initializer_list<AttackedEventModifier*> modifiers)
		: m_health(nullptr),
		  m_modifiers(modifiers)
	{}

	virtual void Init() override;
	virtual void Update() override;
};