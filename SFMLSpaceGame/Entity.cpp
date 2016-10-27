#include "stdafx.h"
#include <Entity.h>
#include <EntityManager.h>

void Entity::Update()
{
	events.Update();

	for (int i = 0; i < maxComponents; i++)
	{
		if (m_componentBitset[i] && m_componentArray[i]->IsActive())
			m_componentArray[i]->Update();
	}

	/*for (auto& c : m_components)
	{
		if (c->active)
			c->Update();
	}*/
}

void Entity::Render(sf::RenderTarget& target, sf::RenderStates& states)
{
	for (int i = 0; i < maxComponents; i++)
	{
		if (m_componentBitset[i] && m_componentArray[i]->IsActive())
			m_componentArray[i]->Render(target, states);
	}
	/*for (auto& c : m_components)
	{
		if (c->active)
			c->Render(target, states);
	}*/
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