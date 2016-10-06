#pragma once

#include <Components/Physics.h>
#include <Entity.h>

class EntitySensor : public Component 
{
private:
	Physics* m_physics{ nullptr };
	Group m_group;
	float m_radius;

	void HandleCollisionWithEntity(Entity* ent);
public:
	EntitySensor(float radius, Group group)
		: m_radius(radius), m_group(group)
	{}

	std::vector<Entity*> sensedEntities;

	virtual void Init() override;
	virtual void Update() override;
};