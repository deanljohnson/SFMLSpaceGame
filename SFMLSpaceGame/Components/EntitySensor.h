#pragma once

#include <Entity.h>
#include <Components/Component.h>
#include <Interfaces/ComponentSensor.h>
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>

class Physics;

class EntitySensor : public Component, public ComponentSensor
{
private:
	bool m_triggered{ false };
	Physics& m_physics;
	b2Fixture* m_sensingFixture{ nullptr };
	std::vector<Group> m_groups;
	float m_radius;
	std::vector<std::function<void(bool, Sensor*)>> m_callbacks{};

	void HandleCollisionWithEntity(Entity* ent);
	void SetTriggered(bool val);

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_radius, m_groups, m_triggered);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<EntitySensor>& construct)
	{
		EntityID selfID;
		float radius;
		std::vector<Group> groups;
		ar(selfID, radius, groups);
		construct(selfID, radius, groups);

		ar(construct->m_triggered);
	}
public:
	EntitySensor(EntityID ent, float radius, const std::vector<Group>& groups);
	
	std::vector<Entity*> sensedEntities;
	virtual void Update() override;

	void SetRange(float range);

	virtual void AddCallback(std::function<void(bool, Sensor*)> callback) override;
	virtual void AttachComponent(Component* c) override;
};