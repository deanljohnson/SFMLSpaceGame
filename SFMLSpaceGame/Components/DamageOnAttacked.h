#pragma once
#include <DefaultSerializeable.h>

class Health;
class AttackedEventModifier;
class AttackedEvent;

class DamageOnAttacked : public Component, public DefaultSerializeable<DamageOnAttacked>, public Updateable
{
private:
	Health& m_health;
	std::vector<AttackedEventModifier*> m_modifiers;

	void HandleAttack(AttackedEvent* event);
public:
	explicit DamageOnAttacked(EntityID ent);
	DamageOnAttacked(EntityID ent, std::initializer_list<AttackedEventModifier*> modifiers);

	virtual void Update() override;

	void AddModifier(AttackedEventModifier* modifier);
};