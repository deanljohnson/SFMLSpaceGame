#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <EntityID.h>
#include <EntityHandle.h>
#include <Group.h>

class EntityManager
{
private:
	EntityID m_nextID = ENTITY_ID_NULL;
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::unordered_map<EntityID, std::shared_ptr<EntityHandle>> m_entityHandles;
	std::array<std::vector<Entity*>, maxGroups> m_groupedEntities;
public:
	void Refresh();
	void Update();
	void Render(sf::RenderTarget& target, sf::RenderStates& states);

	std::shared_ptr<EntityHandle> AddEntity(b2World* world);
	std::shared_ptr<EntityHandle> AddEntity(b2World* world, Group group);
	void InvalidateHandle(EntityID id);

	void AddToGroup(Entity* ent, Group group);
	std::vector<Entity*>& GetEntitiesByGroup(Group group);
};