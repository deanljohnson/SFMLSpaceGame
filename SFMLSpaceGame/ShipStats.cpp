// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <ShipStats.h>

ShipStats::ShipStats()
	: m_shipThrust(),
	m_dirGunData(),
	m_misLauncherData(),
	m_shieldData(),
	m_colliderVertices(),
	m_thrusterLocations(),
	maxHull(0),
	interceptLeadMultiplier(0),
	followDistance(0),
	approachDistance(0),
	strafeDistance(0),
	sensorRange(0)
{}

ShipStats::ShipStats(float hullStrength, float interceptLead,
					float followDistance, float approachDistance,
					float strafeDistance, float sensorRange,
					ShipThrust thrust, DirectionalGunData dirData,
					MissileLauncherData misData, MiningLaserData minLaser, ShieldData shieldData,
					const std::string& imageLocation, const std::vector<sf::Vector2f>& colliderVertices,
					const std::vector<sf::Vector2f>& thrusterLocations)
	: m_shipThrust(thrust),
	m_dirGunData(dirData),
	m_misLauncherData(misData),
	m_miningLaserData(minLaser),
	m_shieldData(shieldData),
	m_colliderVertices(colliderVertices),
	m_thrusterLocations(thrusterLocations),
	maxHull(hullStrength),
	interceptLeadMultiplier(interceptLead),
	followDistance(followDistance),
	approachDistance(approachDistance),
	strafeDistance(strafeDistance),
	sensorRange(sensorRange),
	imageLocation(imageLocation)
{}

std::vector<sf::Vector2f>& ShipStats::GetColliderVertices()
{
	return m_colliderVertices;
}

std::vector<sf::Vector2f>& ShipStats::GetThrusterLocations()
{
	return m_thrusterLocations;
}

ShipThrust* ShipStats::GetShipThrust()
{
	return &m_shipThrust;
}

DirectionalGunData* ShipStats::GetDirGunData()
{
	return &m_dirGunData;
}

MissileLauncherData* ShipStats::GetMissileLauncherData()
{
	return &m_misLauncherData;
}

MiningLaserData* ShipStats::GetMiningLaserData()
{
	return &m_miningLaserData;
}

ShieldData* ShipStats::GetShieldData()
{
	return &m_shieldData;
}

void ShipStats::SetColliderVertices(const std::vector<sf::Vector2f>& colliderVertices)
{
	m_colliderVertices = { colliderVertices };
}

void ShipStats::Copy(ShipStats* other)
{
	maxHull = other->maxHull;

	interceptLeadMultiplier = other->interceptLeadMultiplier;
	followDistance = other->followDistance;
	approachDistance = other->approachDistance;
	strafeDistance = other->strafeDistance;
	sensorRange = other->sensorRange;

	m_shipThrust.Forward = other->m_shipThrust.Forward;
	m_shipThrust.Side = other->m_shipThrust.Side;
	m_shipThrust.Reverse = other->m_shipThrust.Reverse;
	m_shipThrust.Steer = other->m_shipThrust.Steer;

	m_dirGunData.hardPoints = { other->m_dirGunData.hardPoints };
	m_dirGunData.rigs = { other->m_dirGunData.rigs };

	m_misLauncherData.hardPoints = { other->m_misLauncherData.hardPoints };
	m_misLauncherData.rigs = { other->m_misLauncherData.rigs };

	m_miningLaserData.hardPoint = { other->m_miningLaserData.hardPoint };
	m_miningLaserData.rig = { other->m_miningLaserData.rig };

	m_shieldData.FrontStrength = other->m_shieldData.FrontStrength;
	m_shieldData.SideStrength = other->m_shieldData.SideStrength;
	m_shieldData.RearStrength = other->m_shieldData.RearStrength;
	m_shieldData.RegenSpeed = other->m_shieldData.RegenSpeed;

	m_colliderVertices = { other->m_colliderVertices };
	m_thrusterLocations = { other->m_thrusterLocations };
}

ShipStats* ShipStats::Clone(ShipStats* other)
{
	return new ShipStats(other->maxHull,
		other->interceptLeadMultiplier,
		other->followDistance,
		other->approachDistance,
		other->strafeDistance,
		other->sensorRange,
		ShipThrust(other->m_shipThrust),
		DirectionalGunData(other->m_dirGunData),
		MissileLauncherData(other->m_misLauncherData),
		MiningLaserData(other->m_miningLaserData),
		ShieldData(other->m_shieldData),
		other->imageLocation,
		{ other->m_colliderVertices },
		{ other->m_thrusterLocations });
}

std::string ShipStats::GetTypeName()
{ static const std::string name = "ship"; return name; }

ShipStats& ShipStats::operator=(ShipStats other)
{
	using std::swap;
	swap(*this, other);
	return *this;
}