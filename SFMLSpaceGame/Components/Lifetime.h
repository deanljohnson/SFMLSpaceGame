#pragma once
#include <Components/Component.h>
#include <cereal/access.hpp>

class Lifetime : public Component
{
private:
	float m_lifetime;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_lifetime);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Lifetime>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		ar(construct->m_lifetime);
	}
public:
	Lifetime(EntityID ent, float lifetime);

	virtual void Update() override;
};
