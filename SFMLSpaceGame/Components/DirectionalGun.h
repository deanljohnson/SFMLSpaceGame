#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Interfaces/Gun.h>
#include <vector>
#include <ResourceLoader.h>
#include "SoundSource.h"

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
	DirectionalGunData(float cd, float heatLim, float coolingRate, float heatGen, ResourceID shotSoundID, const std::initializer_list<HardPoint>& hardPointLocations)
		: fireRate(cd),
		  heatLimit(heatLim),
		  cooldownRate(coolingRate),
		  heatGenerated(heatGen),
		  hardPoints(hardPointLocations),
		  soundID(shotSoundID)
	{}

	float fireRate; // Time in between two shots
	float heatLimit; // Time before the gun is overheated
	float cooldownRate; // How fast heat is dissipated from the gun per second
	float heatGenerated; // How much heat is generated per shot
	std::vector<HardPoint> hardPoints;
	ResourceID soundID;
};

class DirectionalGun : public Component, Gun
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
	SoundSource* m_shotSound{ nullptr };
	float m_lastFiringTime;
	DirectionalGunData* m_gunData;
	float m_currentHeat{ 0.f };

public:
	explicit DirectionalGun(DirectionalGunData* data)
		: m_gunData(data)
	{}

	virtual void Init() override;
	virtual void Update() override;

	virtual void Shoot() override;

	void SetSoundSource(SoundSource* source);
};