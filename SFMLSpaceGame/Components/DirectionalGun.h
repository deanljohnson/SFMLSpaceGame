#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Interfaces/Gun.h>

class DirectionalGun : public Component, Gun
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
public:
	virtual void Init() override;

	virtual void Shoot();
};