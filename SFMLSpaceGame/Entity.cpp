#include "stdafx.h"
#include <Entity.h>
#include <EntityManager.h>

void Entity::OnDeserialize()
{
	EntityManager::AddEntity(this);
}

void Entity::ApplyInitializers()
{
	for (auto i = 0; i < EntityInitializer::Type::Count; i++)
	{
		if (m_initializerBitset[i])
		{
			Execute(static_cast<EntityInitializer::Type>(i), *this);
		}

	}
}

void Entity::Update()
{
	events.Update();

	for (auto i = 0; i < maxComponents; i++)
	{
		if (m_componentBitset[i] && m_componentArray[i]->IsActive())
			m_componentArray[i]->Update();
	}
}

void Entity::Render(sf::RenderTarget& target, sf::RenderStates& states)
{
	for (auto i = 0; i < maxComponents; i++)
	{
		if (m_componentBitset[i] && m_componentArray[i]->IsActive())
			m_componentArray[i]->Render(target, states);
	}
}

void Entity::OnDestroy() 
{
	if (destroyCallback)
		destroyCallback(this);

	for (auto& c : m_components)
	{
		c->OnDestroy();
	}
}

void Entity::ApplyInitializer(EntityInitializer::Type init)
{
	m_initializerBitset[init] = true;
	Execute(init, *this);
}

bool Entity::HasGroup(Group group) const noexcept
{
	return m_groupBitset[group];
}

void Entity::DeleteFromGroup(Group group) noexcept
{
	m_groupBitset[group] = false;
}

void Entity::AddToGroup(Group group) noexcept 
{
	m_groupBitset[group] = true;
	EntityManager::AddToGroup(this, group);
}