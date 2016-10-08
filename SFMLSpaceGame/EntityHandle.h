#pragma once
#include <EntityID.h>

class Entity;
class EntityManager;

class EntityHandle 
{
private:
	EntityID m_ID;
	Entity* m_entity;
	EntityManager* m_manager;

public:
	EntityHandle(EntityManager* manager, Entity* ent, EntityID id)
		: m_ID(id), m_entity(ent), m_manager(manager)
	{}
	inline bool IsValid();

	Entity* operator->();
};