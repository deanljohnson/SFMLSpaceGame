#pragma once

#include <Entity.h>
#include <Components/Component.h>
#include <Interfaces/ComponentSensor.h>

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
public:
	EntitySensor(EntityID ent, float radius, const std::initializer_list<Group>& groups);

	std::vector<Entity*> sensedEntities;
	virtual void Update() override;

	virtual void AddCallback(std::function<void(bool, Sensor*)> callback) override;
	virtual void AttachComponent(Component* c) override;
};