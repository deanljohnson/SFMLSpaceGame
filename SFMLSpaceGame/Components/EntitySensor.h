#pragma once

#include <Components/Physics.h>
#include <Entity.h>

class EntitySensor : public Component 
{
private:
	bool m_triggered{ false };
	Physics* m_physics{ nullptr };
	b2Fixture* m_sensingFixture{ nullptr };
	std::vector<Group> m_groups;
	float m_radius;
	std::vector<std::function<void(bool, EntitySensor*)>> m_callbacks{};

	void HandleCollisionWithEntity(Entity* ent);
	void SetTriggered(bool val);
public:
	EntitySensor(float radius, std::initializer_list<Group> groups)
		: m_groups(groups), m_radius(radius)
	{}

	std::vector<Entity*> sensedEntities;

	virtual void Init() override;
	virtual void Update() override;

	inline void AddCallback(std::function<void(bool, EntitySensor*)> callback) 
	{
		m_callbacks.push_back(callback);
	}
};