#include <EntityHandle.h>
#include "Entity.h"
#include <EntityManager.h>

bool EntityHandle::IsValid()
{
	return m_manager->IsValidID(m_ID);
}

Entity* EntityHandle::operator->()
{
	return m_entity;
}