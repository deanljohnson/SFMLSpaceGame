#include <Entity.h>
#include <EntityManager.h>

void Entity::Update()
{
	for (auto& c : m_components) 
		c->Update();
}

void Entity::Render(sf::RenderTarget& target)
{
	for (auto& c : m_components) 
		c->Render(target);
}

template<typename T>
bool Entity::HasComponent() const
{
	return m_componentBitset[GetComponentTypeID<T>()];
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
	m_manager.AddToGroup(this, group);
}