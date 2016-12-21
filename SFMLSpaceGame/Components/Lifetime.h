#pragma once
#include <cereal/access.hpp>

class Lifetime : public Component, public Updateable
{
private:
	float m_maxLifetime;
	float m_lifetime;
	bool m_destroy;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_maxLifetime, m_lifetime, m_destroy);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Lifetime>& construct)
	{
		EntityID selfID;
		float maxLifetime;
		float lifetime;
		bool destroy;
		ar(selfID, maxLifetime, lifetime, destroy);
		construct(selfID, maxLifetime, destroy);
		construct->m_lifetime = lifetime;
	}
public:
	Lifetime(EntityID ent, float lifetime, bool destoryOnTimeExpire = true);

	virtual void Update() override;
	void Reset();
};
