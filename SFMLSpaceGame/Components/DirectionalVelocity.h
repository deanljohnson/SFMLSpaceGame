#pragma once
#include <Components/Component.h>
#include <Components/Physics.h>
#include <Components/DirectionalKeyboardInput.h>

class DirectionalVelocity : public Component, public Updateable
{
private:
	DirectionalKeyboardInput& m_input;
	Physics& m_physics;
	float m_maxVelocity;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_maxVelocity);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<DirectionalVelocity>& construct)
	{
		EntityID selfID;
		float maxVel;
		ar(selfID, maxVel);
		construct(selfID, maxVel);
	}

public:
	DirectionalVelocity(EntityID ent, float maxVel);

	virtual void Update() override;
};