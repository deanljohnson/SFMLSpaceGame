#pragma once
#include <Entity.h>
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
		ar(entity.GetID(), m_maxHealth, m_health);
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
	explicit Health(EntityID ent)
		: Component(ent),
		  m_maxHealth(100.f),
		  m_health(100.f)
	{
	}

	inline float GetHealth() const { return m_health; }
	inline void SetHealth(float hp)
	{
		m_health = std::max(0.f, hp);
		if (m_health <= 0.f)
			entity->Destroy();
	}
	inline void RemoveHealth(float amt) { SetHealth(m_health - amt); }
	inline float GetHealthNormalized() { return m_health / m_maxHealth; }
};