#include <EntityManager.h>
#include <Entity.h>

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
	for (auto i(0u); i < maxGroups; i++)
	{
		auto& curGroup{ m_groupedEntities[i] };

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

std::shared_ptr<EntityHandle> EntityManager::AddEntity(b2World* world)
{
	Entity* e{ new Entity(this, world, m_nextID++) };
	std::unique_ptr<Entity> uPtr(e);

	EntityHandle* handle = new EntityHandle(e);
	std::shared_ptr<EntityHandle> handPtr(handle);

	m_entities.emplace_back(move(uPtr));
	m_entityHandles.emplace(std::make_pair(e->GetID(), handPtr));
	return handPtr;
}

std::shared_ptr<EntityHandle> EntityManager::AddEntity(b2World* world, Group group)
{
	Entity* e{ new Entity(this, world, m_nextID++) };
	std::unique_ptr<Entity> uPtr(e);

	EntityHandle* handle = new EntityHandle(e);
	std::shared_ptr<EntityHandle> handPtr(handle);

	m_entities.emplace_back(move(uPtr));
	m_entityHandles.emplace(std::make_pair(e->GetID(), handPtr));

	e->AddToGroup(group);

	return handPtr;
}

void EntityManager::InvalidateHandle(EntityID id)
{
	auto it = m_entityHandles.find(id);
	if (it != m_entityHandles.end()) {
		it->second->Invalidate();
	}
}

void EntityManager::AddToGroup(Entity* ent, Group group)
{
	m_groupedEntities[group].emplace_back(ent);
}

std::vector<Entity*>& EntityManager::GetEntitiesByGroup(Group group)
{
	return m_groupedEntities[group];
}
