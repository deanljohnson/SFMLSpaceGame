#include "stdafx.h"
#include <EntityManager.h>
#include <Entity.h>

EntityID EntityManager::m_nextID = ENTITY_ID_NULL;
std::vector<std::unique_ptr<Entity>> EntityManager::m_entities{};
std::unordered_map<EntityID, std::unique_ptr<EntityHandle>> EntityManager::m_entityHandles{};
std::array<std::vector<Entity*>, maxGroups> EntityManager::m_groupedEntities{};

void EntityManager::Clear()
{
	for (auto i(0u); i < maxGroups; i++)
	{
		auto& v(m_groupedEntities[i]);
		v.clear();
	}

	m_entityHandles.clear();
	m_entities.clear();
}

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
				if (!entity->isAlive())
				{
					m_entityHandles.erase(entity->GetID());
					return true;
				}
				return false;
			}), end(m_entities)
	);
}

void EntityManager::Update()
{
	for (auto i(0u); i < maxGroups; i++)
	{
		auto& curGroup( m_groupedEntities[i] );

		for (auto& e : curGroup)
			e->Update();
	}
}

void EntityManager::Render(sf::RenderTarget& target, sf::RenderStates& states)
{
	for (auto i(0u); i < maxGroups; i++)
	{
		auto& curGroup(m_groupedEntities[i]);

		for (auto& e : curGroup)
			e->Render(target, states);
	}
}

EntityHandle EntityManager::AddEntity(Entity* e)
{
	if (m_nextID <= e->GetID())
		m_nextID = e->GetID() + 1;

	std::unique_ptr<Entity> uPtr(e);

	EntityHandle* hanPtr{ new EntityHandle(e, e->GetID()) };
	std::unique_ptr<EntityHandle> handle(hanPtr);

	m_entities.emplace_back(move(uPtr));
	m_entityHandles.emplace(make_pair(e->GetID(), move(handle)));

	Group g = 0;
	for (auto& group : m_groupedEntities)
	{
		if (e->HasGroup(g))
			e->AddToGroup(g);
		g++;
	}

	return EntityHandle(*hanPtr);
}


EntityHandle EntityManager::AddEntity()
{
	Entity* e{ new Entity(m_nextID++) };
	std::unique_ptr<Entity> uPtr(e);

	EntityHandle* hanPtr{ new EntityHandle(e, e->GetID()) };
	std::unique_ptr<EntityHandle> handle(hanPtr);

	m_entities.emplace_back(move(uPtr));
	m_entityHandles.emplace(make_pair(e->GetID(), move(handle)));

	return EntityHandle(*hanPtr);
}

EntityHandle EntityManager::AddEntity(Group group)
{
	Entity* e{ new Entity(m_nextID++) };
	std::unique_ptr<Entity> uPtr(e);

	EntityHandle* hanPtr{ new EntityHandle(e, e->GetID()) };
	std::unique_ptr<EntityHandle> handle(hanPtr);

	m_entities.emplace_back(move(uPtr));
	m_entityHandles.emplace(make_pair(e->GetID(), move(handle)));

	e->AddToGroup(group);

	return EntityHandle(*hanPtr);
}

bool EntityManager::IsValidID(EntityID id)
{
	return m_entityHandles.find(id) != m_entityHandles.end();
}

EntityHandle EntityManager::Get(EntityID id)
{
	return *m_entityHandles.find(id)->second.get();
}

void EntityManager::AddToGroup(Entity* ent, Group group)
{
	m_groupedEntities[group].emplace_back(ent);
}

const std::vector<Entity*>& EntityManager::GetEntitiesByGroup(Group group)
{
	return m_groupedEntities[group];
}
