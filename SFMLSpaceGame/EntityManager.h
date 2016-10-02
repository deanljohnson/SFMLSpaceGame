#pragma once
#include <vector>
#include <memory>
#include <Entity.h>

class EntityManager
{
private:
	std::vector<std::unique_ptr<Entity>> m_entities;

	std::array<std::vector<Entity*>, maxGroups> m_groupedEntities;
public:
	void Refresh();
	void Update();
	void Render(sf::RenderTarget& target, sf::RenderStates& states);

	Entity* AddEntity(b2World* world);
	Entity* AddEntity(b2World* world, Group group);

	void AddToGroup(Entity* ent, Group group);
	std::vector<Entity*>& GetEntitiesByGroup(Group group);
};