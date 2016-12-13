#pragma once
#include <EntityID.h>
#include <cereal/access.hpp>

class Health : public Component
{
private:
	float m_maxHealth;
	float m_health;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(GetEntityID(), m_maxHealth, m_health);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Health>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		ar(construct->m_maxHealth, construct->m_health);
	}
public:
	explicit Health(EntityID ent);

	float GetHealth() const;
	float GetHealthNormalized() const;

	void SetHealth(float hp);
	void RemoveHealth(float amt);
};