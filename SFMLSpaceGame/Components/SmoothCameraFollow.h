#pragma once
#include <Components/Position.h>

class SmoothCameraFollow : public Component
{
private:
	Position* m_position{ nullptr };
	sf::Vector2f m_LastPosition;

public:
	virtual void Init() override;
	virtual void Update() override;
};