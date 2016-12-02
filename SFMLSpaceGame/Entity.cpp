// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
		// Does the entity need this initializer applied?
		if (m_initializerBitset[i])
		{
			// Turn the loop counter into an initializer type
			// and execute it
			Execute(static_cast<EntityInitializer::Type>(i), *this);
		}
	}
}

void Entity::Update()
{
	events.Update();

	for (auto& c : m_components)
	{
		if (c.second->IsActive())
			c.second->Update();
	}
}

void Entity::Render(sf::RenderTarget& target, sf::RenderStates& states)
{
	for (auto& c : m_components)
	{
		if (c.second->IsActive())
			c.second->Render(target, states);
	}
}

void Entity::OnDestroy() 
{
	if (destroyCallback)
		destroyCallback(this);

	for (auto& c : m_components)
	{
		c.second->OnDestroy();
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