#pragma once
#include <Entity.h>

class EntityHandle 
{
private:
	Entity* m_entity;
	bool m_valid;

public:
	EntityHandle(Entity* entity)
		: m_entity(entity)
	{}
	inline bool IsValid() { return m_valid; }
	inline void Invalidate() { m_valid = false; }

	Entity* operator->()
	{
		if (!m_valid) return nullptr;
		return m_entity;
	}

	/*Entity &operator->() const
	{
		return *m_entity;
	}*/
};