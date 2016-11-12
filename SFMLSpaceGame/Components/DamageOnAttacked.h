#pragma once
#include <Components/Health.h>
#include <Interfaces/AttackEventModifier.h>
#include <cereal/access.hpp>

class DamageOnAttacked : public Component
{
private:
	Health& m_health;
	std::vector<AttackedEventModifier*> m_modifiers;

	void HandleAttack(Event& event);

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID());
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<DamageOnAttacked>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);
	}
public:
	explicit DamageOnAttacked(EntityID ent);
	DamageOnAttacked(EntityID ent, std::initializer_list<AttackedEventModifier*> modifiers);

	virtual void Update() override;

	void AddModifier(AttackedEventModifier* modifier);
};