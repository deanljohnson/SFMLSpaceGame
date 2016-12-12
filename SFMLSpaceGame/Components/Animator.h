#pragma once
#include <DefaultSerializeable.h>
#include <Components\Sprite.h>
#include <cereal\access.hpp>
#include <Animation.h>
#include <EntityID.h>
#include <Components/Interfaces/Updateable.h>


class Animator : public Component, public DefaultSerializeable<Animator>, public Updateable
{
private:
	Sprite& m_sprite;

	Animation m_animation;
	std::string m_id;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_id, entity->GetComponentID<Sprite>(m_sprite));
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Animator>& construct)
	{
		EntityID selfID;
		std::string id;
		ComponentID sprID;
		ar(selfID, id, sprID);
		construct(selfID, id, sprID);
	}
public:
	explicit Animator(EntityID ent, const std::string& id, ComponentID spriteID = 0);

	virtual void Update() override;

	float GetLength();
	float GetSpeed();
};