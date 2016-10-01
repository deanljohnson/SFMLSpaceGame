#pragma once
#include <Components/Position.h>

class SmoothCameraFollow : public Component
{
private:
	Position* m_position{ nullptr };
	b2Vec2 m_LastPosition;

public:
	virtual void Init() override;
	virtual void Update() override;
};