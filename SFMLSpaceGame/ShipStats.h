#pragma once
#include <Components/ShipThrusters.h>
#include <Components/DirectionalGun.h>
#include <Components/MissileLauncher.h>
#include <Components/MiningLaser.h>
#include <Components/Shields.h>
#include <cereal/types/vector.hpp> // Need for collider vertices serialization
#include <SFMLSerialization.h> // Need for collider vertices serialization

class ShipStats
{
private:
	ShipThrust m_shipThrust;
	DirectionalGunData m_dirGunData;
	MissileLauncherData m_misLauncherData;
	MiningLaserData m_miningLaserData;
	ShieldData m_shieldData;
	std::vector<sf::Vector2f> m_colliderVertices;
	std::vector<sf::Vector2f> m_thrusterLocations;

	ShipStats(float hullStrength, float interceptLead, float followDistance,
		float approachDistance, float strafeDistance,
		float sensorRange, ShipThrust thrust,
		DirectionalGunData dirData, MissileLauncherData misData,
		MiningLaserData minLaser,
		ShieldData shieldData, const std::string& imageLocation,
		const std::vector<sf::Vector2f>& colliderVertices,
		const std::vector<sf::Vector2f>& thrusterLocations);

public:
	float maxHull;
	float interceptLeadMultiplier;
	float followDistance;
	float approachDistance;
	float strafeDistance;
	float sensorRange;
	std::string imageLocation;

	ShipStats();

	std::vector<sf::Vector2f>& GetColliderVertices();
	std::vector<sf::Vector2f>& GetThrusterLocations();

	ShipThrust* GetShipThrust();
	DirectionalGunData* GetDirGunData();
	MissileLauncherData* GetMissileLauncherData();
	MiningLaserData* GetMiningLaserData();
	ShieldData* GetShieldData();

	void SetColliderVertices(const std::vector<sf::Vector2f>& colliderVertices);
	void SetThrusterLocations(const std::vector<sf::Vector2f>& thrusterLocations) { m_thrusterLocations = { thrusterLocations }; }

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("HullStrength", maxHull),
				cereal::make_nvp("InterLeadMult", interceptLeadMultiplier),
				cereal::make_nvp("FollowDist", followDistance),
				cereal::make_nvp("ApproachDist", approachDistance),
				cereal::make_nvp("StrafeDist", strafeDistance),
				cereal::make_nvp("SensorRange", sensorRange),
				cereal::make_nvp("ShipThrust", m_shipThrust),
				cereal::make_nvp("DirGunData", m_dirGunData),
				cereal::make_nvp("MisLauncherData", m_misLauncherData),
				cereal::make_nvp("MinLaserData", m_miningLaserData),
				cereal::make_nvp("Shields", m_shieldData),
				cereal::make_nvp("ImageLocation", imageLocation),
				cereal::make_nvp("ColliderVertices", m_colliderVertices),
				cereal::make_nvp("ThrusterLocations", m_thrusterLocations));
	}

	void Copy(ShipStats* other);
	static ShipStats* Clone(ShipStats* other);

	static std::string GetTypeName();

	ShipStats& operator=(ShipStats other);
};