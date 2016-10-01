#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Interfaces/Gun.h>
#include <vector>

struct HardPoint
{
	HardPoint(b2Vec2 offset, float angle)
		: positionOffset(offset),
		  angleOffset(angle)
	{}

	//Offset from the ships center to the HardPoint location. 
	//+x = towards the ships nose 
	//+y = towards the ships right side
	b2Vec2 positionOffset;
	float angleOffset;
};

struct DirectionalGunData
{
	DirectionalGunData(float cd, std::initializer_list<HardPoint> hardPointLocations)
		: cooldown(cd), 
		  hardPoints(hardPointLocations)
	{}

	float cooldown;
	std::vector<HardPoint> hardPoints;
};

class DirectionalGun : public Component, Gun
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
	float m_cooldown;
	float m_lastFiringTime;
	std::vector<HardPoint> m_hardPoints;

public:
	explicit DirectionalGun(DirectionalGunData data)
		: m_cooldown(data.cooldown),
		  m_hardPoints(data.hardPoints)
	{}

	virtual void Init() override;

	virtual void Shoot() override;
};