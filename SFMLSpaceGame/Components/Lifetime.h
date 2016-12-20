#pragma once
#include <cereal/access.hpp>

class Lifetime : public Component, public Updateable
{
private:
	float m_lifetime;
	float m_destroy;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_lifetime, m_destroy);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Lifetime>& construct)
	{
		EntityID selfID;
		float lifetime;
		bool destroy;
		ar(selfID, lifetime, destroy);
		construct(selfID, lifetime, destroy);
	}
public:
	Lifetime(EntityID ent, float lifetime, bool destoryOnTimeExpire = true);

	virtual void Update() override;
};
