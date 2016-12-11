#pragma once
#include <Components/ShipThrusters.h>
#include <Components/DirectionalGun.h>
#include <Components/MissileLauncher.h>
#include <Components/MiningLaser.h>
#include <Components/Shields.h>
#include <cereal/types/vector.hpp> // Need for collider vertices serialization
#include <SFMLSerialization.h> // Need for collider vertices serialization
#include <SpriteKey.h>

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
		ShieldData shieldData, const SpriteKey& imageLocation,
		const std::vector<sf::Vector2f>& colliderVertices,
		const std::vector<sf::Vector2f>& thrusterLocations);

public:
	float hullStrength;
	float interceptLeadMultiplier;
	float followDistance;
	float approachDistance;
	float strafeDistance;
	float sensorRange;
	SpriteKey imageLocation;

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
		archive(cereal::make_nvp("hullStrength", hullStrength),
				cereal::make_nvp("interLeadMult", interceptLeadMultiplier),
				cereal::make_nvp("followDist", followDistance),
				cereal::make_nvp("approachDist", approachDistance),
				cereal::make_nvp("strafeDist", strafeDistance),
				cereal::make_nvp("sensorRange", sensorRange),
				cereal::make_nvp("shipThrust", m_shipThrust),
				cereal::make_nvp("dirGunData", m_dirGunData),
				cereal::make_nvp("misLauncherData", m_misLauncherData),
				cereal::make_nvp("minLaserData", m_miningLaserData),
				cereal::make_nvp("shields", m_shieldData),
				cereal::make_nvp("imageLocation", imageLocation),
				cereal::make_nvp("colliderVertices", m_colliderVertices),
				cereal::make_nvp("thrusterLocations", m_thrusterLocations));
	}

	void Copy(ShipStats* other);
	static ShipStats* Clone(ShipStats* other);

	static std::string GetTypeName();

	ShipStats& operator=(ShipStats other);
};