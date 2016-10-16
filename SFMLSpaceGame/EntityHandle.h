#pragma once
#include <EntityID.h>

class Entity;
class EntityManager;

class EntityHandle 
{
public:
	EntityHandle& operator=(const EntityHandle& other);
	EntityHandle& operator=(EntityHandle&& other);
	EntityHandle(const EntityHandle& other);
	EntityHandle(EntityHandle&& other);

private:
	EntityID m_ID;
	Entity* m_entity;

public:
	EntityHandle() 
	  : m_ID(0), m_entity(nullptr) {}
	EntityHandle(Entity* ent, EntityID id);

	bool IsValid();

	Entity* operator->();

	EntityID GetID() const { return m_ID; };
};