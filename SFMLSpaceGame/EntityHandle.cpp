#include <EntityManager.h>

/*EntityHandle::EntityHandle()
	: m_ID(ENTITY_ID_NULL), m_entity(nullptr), m_manager(nullptr)
{}*/
EntityHandle::EntityHandle(EntityManager* manager, Entity* ent, EntityID id)
	: m_ID(id), m_entity(ent), m_manager(manager)
{}
EntityHandle::EntityHandle(const EntityHandle& other)
	: m_ID{ other.m_ID },
	  m_entity{ other.m_entity },
	  m_manager{ other.m_manager }
{
}
EntityHandle::EntityHandle(EntityHandle&& other)
	: m_ID{ other.m_ID },
	  m_entity{ other.m_entity },
	  m_manager{ other.m_manager }
{
}
EntityHandle& EntityHandle::operator=(const EntityHandle& other)
{
	if (this == &other)
		return *this;
	m_ID = other.m_ID;
	m_entity = other.m_entity;
	m_manager = other.m_manager;
	return *this;
}
EntityHandle& EntityHandle::operator=(EntityHandle&& other)
{
	if (this == &other)
		return *this;
	m_ID = other.m_ID;
	m_entity = other.m_entity;
	m_manager = other.m_manager;
	return *this;
}

bool EntityHandle::IsValid()
{
	if (m_manager == nullptr) return false;
	return m_manager->IsValidID(m_ID);
}

Entity* EntityHandle::operator->()
{
	return m_entity;
}