#pragma once

#include <Entity.h>
#include <Components/Component.h>
#include <Interfaces/ComponentSensor.h>

class Physics;

class EntitySensor : public Component, public ComponentSensor
{
private:
	bool m_triggered{ false };
	Physics* m_physics{ nullptr };
	b2Fixture* m_sensingFixture{ nullptr };
	std::vector<Group> m_groups;
	float m_radius;
	std::vector<std::function<void(bool, Sensor*)>> m_callbacks{};

	void HandleCollisionWithEntity(Entity* ent);
	void SetTriggered(bool val);
public:
	EntitySensor(float radius, const std::initializer_list<Group>& groups)
		: m_groups(groups), m_radius(radius)
	{}

	std::vector<Entity*> sensedEntities;

	virtual void Init() override;
	virtual void Update() override;

	virtual void AddCallback(std::function<void(bool, Sensor*)> callback) override;
	virtual void AttachComponent(Component* c) override;
};