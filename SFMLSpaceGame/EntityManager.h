#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <array>
#include <EntityID.h>
#include <EntityHandle.h>
#include <Entity.h>
#include <Group.h>

namespace sf{
	class RenderTarget;
	class RenderStates;
}

class b2World;

class EntityManager
{
private:
	EntityID m_nextID = ENTITY_ID_NULL;
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::unordered_map<EntityID, std::unique_ptr<EntityHandle>> m_entityHandles;
	std::array<std::vector<Entity*>, maxGroups> m_groupedEntities;
public:
	void Refresh();
	void Update();
	void Render(sf::RenderTarget& target, sf::RenderStates& states);

	inline bool IsValidID(EntityID id);
	inline EntityHandle& Get(EntityID id);

	EntityHandle& AddEntity(b2World* world);
	EntityHandle& AddEntity(b2World* world, Group group);

	void AddToGroup(Entity* ent, Group group);
	std::vector<Entity*>& GetEntitiesByGroup(Group group);
};