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
	static EntityID m_nextID;
	static std::vector<std::unique_ptr<Entity>> m_entities;
	static std::unordered_map<EntityID, std::unique_ptr<EntityHandle>> m_entityHandles;
	static std::array<std::vector<Entity*>, maxGroups> m_groupedEntities;

public:
	static void Refresh();
	static void Update();
	static void Render(sf::RenderTarget& target, sf::RenderStates& states);

	static bool IsValidID(EntityID id);
	static EntityHandle Get(EntityID id);

	static EntityHandle AddEntity(b2World* world);
	static EntityHandle AddEntity(b2World* world, Group group);

	static void AddToGroup(Entity* ent, Group group);
	static std::vector<Entity*>& GetEntitiesByGroup(Group group);
};