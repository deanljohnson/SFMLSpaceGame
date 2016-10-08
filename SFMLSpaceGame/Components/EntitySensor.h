#pragma once

#include <Components/Physics.h>
#include <Entity.h>

class EntitySensor : public Component 
{
private:
	Physics* m_physics{ nullptr };
	b2Fixture* m_sensingFixture{ nullptr };
	std::vector<Group> m_groups;
	float m_radius;

	void HandleCollisionWithEntity(Entity* ent);
public:
	EntitySensor(float radius, std::initializer_list<Group> groups)
		: m_groups(groups), m_radius(radius)
	{}

	std::vector<Entity*> sensedEntities;

	virtual void Init() override;
	virtual void Update() override;
};