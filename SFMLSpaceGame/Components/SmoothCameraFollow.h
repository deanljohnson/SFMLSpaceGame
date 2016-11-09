#pragma once
#include <Components/Position.h>

class SmoothCameraFollow : public Component
{
private:
	Position& m_position;
	b2Vec2 m_LastPosition;

public:
	explicit SmoothCameraFollow(EntityID ent);
	virtual void Update() override;
};