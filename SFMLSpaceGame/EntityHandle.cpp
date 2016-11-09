#include "stdafx.h"
#include <EntityManager.h>

EntityHandle::EntityHandle(Entity* ent, EntityID id)
	: m_ID(id), m_entity(ent)
{}
EntityHandle::EntityHandle(const EntityHandle& other)
	: m_ID{ other.m_ID },
	  m_entity{ other.m_entity }
{
}
EntityHandle::EntityHandle(EntityHandle&& other)
	: m_ID{ other.m_ID },
	  m_entity{ other.m_entity }
{
}
EntityHandle& EntityHandle::operator=(const EntityHandle& other)
{
	if (this == &other)
		return *this;
	m_ID = other.m_ID;
	m_entity = other.m_entity;
	return *this;
}
EntityHandle& EntityHandle::operator=(EntityHandle&& other)
{
	if (this == &other)
		return *this;
	m_ID = other.m_ID;
	m_entity = other.m_entity;
	return *this;
}

bool EntityHandle::IsValid()
{
	return EntityManager::IsValidID(m_ID);
}

Entity* EntityHandle::operator->()
{
	return m_entity;
}

Entity* EntityHandle::GetRawPointer()
{
	return m_entity;
}