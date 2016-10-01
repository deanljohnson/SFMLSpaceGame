#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Interfaces/Gun.h>
#include <vector>

struct DirectionalGunData
{
	DirectionalGunData(float cd, std::initializer_list<b2Vec2> hardPointLocations)
		: cooldown(cd), 
		  hardPoints(hardPointLocations)
	{}

	float cooldown;
	std::vector<b2Vec2> hardPoints;
};

class DirectionalGun : public Component, Gun
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
	float m_cooldown;
	float m_lastFiringTime;
	std::vector<b2Vec2> m_hardPoints;

public:
	explicit DirectionalGun(DirectionalGunData data)
		: m_cooldown(data.cooldown),
		  m_hardPoints(data.hardPoints)
	{}

	virtual void Init() override;

	virtual void Shoot() override;
};