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
	EntityHandle();
	EntityHandle(Entity* ent, EntityID id);

	bool IsValid();

	Entity* operator->();
	const Entity* operator->() const;
	Entity* GetRawPointer();

	EntityID GetID() const;;
};