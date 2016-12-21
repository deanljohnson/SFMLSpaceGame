#pragma once
#include <EntityHandle.h>
#include <cereal/access.hpp>

// Prevents collisions between this entity and one other
class CollisionFilterComponent : public Component
{
private:
	EntityHandle m_noCollision;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_noCollision.GetID());
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<CollisionFilterComponent>& construct)
	{
		EntityID selfID;
		EntityID noCollisionID;
		ar(selfID, noCollisionID);
		construct(selfID, noCollisionID);
	}
public:
	CollisionFilterComponent(EntityID ent, EntityID noCollision);
	bool ShouldCollide(Entity* other);
	void SetNoCollisionEntity(EntityID noCollision);
};