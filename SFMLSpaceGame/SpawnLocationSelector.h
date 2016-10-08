#pragma once

struct b2Vec2;

class SpawnLocationSelector
{
private:
	enum { AtSpawner } m_type;
public:
	SpawnLocationSelector()
		: m_type(AtSpawner)
	{}
	b2Vec2 Select(const b2Vec2& spawnerLocation);
};