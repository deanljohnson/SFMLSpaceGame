#pragma once
#include <Components/Health.h>
#include <DefaultSerializeable.h>
#include <Interfaces/AttackEventModifier.h>

class DamageOnAttacked : public Component, public DefaultSerializeable<DamageOnAttacked>
{
private:
	Health& m_health;
	std::vector<AttackedEventModifier*> m_modifiers;

	void HandleAttack(Event& event);
public:
	explicit DamageOnAttacked(EntityID ent);
	DamageOnAttacked(EntityID ent, std::initializer_list<AttackedEventModifier*> modifiers);

	virtual void Update() override;

	void AddModifier(AttackedEventModifier* modifier);
};