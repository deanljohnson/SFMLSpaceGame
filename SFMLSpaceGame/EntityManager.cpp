#include "EntityManager.h"

void EntityManager::Refresh()
{
	//Remove entities from groups they are not in, or when they are dead
	for (auto i(0u); i < maxGroups; i++)
	{
		auto& v(m_groupedEntities[i]);

		v.erase(
			std::remove_if(begin(v), end(v),
				[i](Entity* ent)
				{
					return !ent->isAlive() || !ent->HasGroup(i);
				}), end(v)
		);
	}

	//Remove dead entities
	m_entities.erase(
		std::remove_if(begin(m_entities), end(m_entities),
			[](const std::unique_ptr<Entity>& entity)
			{
				return !entity->isAlive();
			}), end(m_entities)
	);
}


void EntityManager::Update()
{
	for (auto& e : m_entities)
		e->Update();
}

void EntityManager::Render(sf::RenderTarget& target)
{
	for (auto& e : m_entities)
		e->Render(target);
}

Entity* EntityManager::AddEntity(b2World& world)
{
	Entity* e{ new Entity(*this, world) };
	std::unique_ptr<Entity> uPtr(e);
	m_entities.emplace_back(move(uPtr));
	return e;
}

void EntityManager::AddToGroup(Entity* ent, Group group)
{
	m_groupedEntities[group].emplace_back(ent);
}

std::vector<Entity*>& EntityManager::GetEntitiesByGroup(Group group)
{
	return m_groupedEntities[group];
}
