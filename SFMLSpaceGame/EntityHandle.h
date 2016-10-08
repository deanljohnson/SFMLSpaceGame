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
	EntityManager* m_manager;

public:
	EntityHandle();
	EntityHandle(EntityManager* manager, Entity* ent, EntityID id);

	bool IsValid();

	Entity* operator->();
};