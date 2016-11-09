#pragma once
#include <EntityID.h>
//#include <string>

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
	//std::string m_name;
	Entity* m_entity;

public:
	EntityHandle() 
	  : m_ID(0), m_entity(nullptr) {}
	EntityHandle(Entity* ent, EntityID id);

	bool IsValid();

	Entity* operator->();
	Entity* GetRawPointer();

	EntityID GetID() const { return m_ID; };
};