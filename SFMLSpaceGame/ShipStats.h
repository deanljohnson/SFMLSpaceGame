#pragma once
#include <Components/ShipThrusters.h>
#include <Components/DirectionalGun.h>
#include <Components/MissileLauncher.h>
#include <Components/Shields.h>
#include <cereal/types/vector.hpp> // Need for collider vertices serialization
#include <SFMLSerialization.h> // Need for collider vertices serialization

class ShipStats
{
private:
	float m_maxHull;
	float m_interceptLeadMultiplier;
	float m_followDistance;
	float m_approachDistance;
	float m_strafeDistance;
	float m_sensorRange;
	ShipThrust m_shipThrust;
	DirectionalGunData m_dirGunData;
	MissileLauncherData m_misLauncherData;
	ShieldData m_shieldData;
	std::string m_imageLocation;
	std::vector<sf::Vector2f> m_colliderVertices;
	std::vector<sf::Vector2f> m_thrusterLocations;

public:
	ShipStats() 
		: m_maxHull(0),
		  m_interceptLeadMultiplier(0),
		  m_followDistance(0),
		  m_approachDistance(0),
		  m_strafeDistance(0),
		  m_sensorRange(0),
		  m_shipThrust(),
		  m_dirGunData(),
		  m_misLauncherData(),
		  m_shieldData(),
		  m_colliderVertices(),
		  m_thrusterLocations()
	{}

	ShipStats(const ShipStats& other)
		: m_maxHull{other.m_maxHull},
		  m_interceptLeadMultiplier{other.m_interceptLeadMultiplier},
		  m_followDistance{other.m_followDistance},
		  m_approachDistance{other.m_approachDistance},
		  m_strafeDistance{other.m_strafeDistance},
		  m_sensorRange{other.m_sensorRange},
		  m_shipThrust{other.m_shipThrust},
		  m_dirGunData{other.m_dirGunData},
		  m_misLauncherData{other.m_misLauncherData},
		  m_shieldData{other.m_shieldData},
		  m_imageLocation{other.m_imageLocation},
		  m_colliderVertices{other.m_colliderVertices},
		  m_thrusterLocations{other.m_thrusterLocations}
	{}

	ShipStats(float hullStrength, float interceptLead, float followDistance,
			float approachDistance, float strafeDistance,
			float sensorRange, ShipThrust thrust,
			DirectionalGunData dirData, MissileLauncherData misData,
		    ShieldData shieldData, const std::string& imageLocation,
			const std::vector<sf::Vector2f>& colliderVertices,
			const std::vector<sf::Vector2f>& thrusterLocations)
		: m_maxHull(hullStrength),
		  m_interceptLeadMultiplier(interceptLead),
		  m_followDistance(followDistance),
		  m_approachDistance(approachDistance),
		  m_strafeDistance(strafeDistance),
		  m_sensorRange(sensorRange),
		  m_shipThrust(thrust),
		  m_dirGunData(dirData),
		  m_misLauncherData(misData),
		  m_shieldData(shieldData),
		  m_imageLocation(imageLocation),
		  m_colliderVertices(colliderVertices),
		  m_thrusterLocations(thrusterLocations)
	{}

	inline float GetMaxHullStrength() const { return m_maxHull; }
	inline float GetInterceptLeadMultiplier() const { return m_interceptLeadMultiplier; }
	inline float GetFollowDistance() const { return m_followDistance; }
	inline float GetApproachDistance() const { return m_approachDistance; }
	inline float GetStrafeDistance() const { return m_strafeDistance; }
	inline float GetSensorRange() const { return m_sensorRange; }
	inline ShipThrust* GetShipThrust() { return &m_shipThrust; }
	inline DirectionalGunData* GetDirGunData() { return &m_dirGunData; }
	inline MissileLauncherData* GetMissileLauncherData() { return &m_misLauncherData; }
	inline ShieldData* GetShieldData() { return &m_shieldData; }
	inline std::string GetImageLocation() const { return m_imageLocation; }
	inline std::vector<sf::Vector2f>& GetColliderVertices() { return m_colliderVertices; }
	inline std::vector<sf::Vector2f>& GetThrusterLocations() { return m_thrusterLocations; }

	inline void SetMaxHullStrength(float val) { m_maxHull = val; }
	inline void SetInterceptLeadMultiplier(float val) { m_interceptLeadMultiplier = val; }
	inline void SetFollowDistance(float val) { m_followDistance = val; }
	inline void SetApproachDistance(float val) { m_approachDistance = val; }
	inline void SetStrafeDistance(float val) { m_strafeDistance = val; }
	inline void SetSensorRange(float val) { m_sensorRange = val; }
	inline void SetImageLocation(const std::string& val) { m_imageLocation = val; }
	inline void SetColliderVertices(const std::vector<sf::Vector2f>& colliderVertices) { m_colliderVertices = { colliderVertices }; }
	inline void SetThrusterLocations(const std::vector<sf::Vector2f>& thrusterLocations) { m_thrusterLocations = { thrusterLocations }; }

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("HullStrength", m_maxHull),
				cereal::make_nvp("InterLeadMult", m_interceptLeadMultiplier),
				cereal::make_nvp("FollowDist", m_followDistance),
				cereal::make_nvp("ApproachDist", m_approachDistance),
				cereal::make_nvp("StrafeDist", m_strafeDistance),
				cereal::make_nvp("SensorRange", m_sensorRange),
				cereal::make_nvp("ShipThrust", m_shipThrust),
				cereal::make_nvp("DirGunData", m_dirGunData),
				cereal::make_nvp("MisLauncherData", m_misLauncherData),
				cereal::make_nvp("Shields", m_shieldData),
				cereal::make_nvp("ImageLocation", m_imageLocation),
				cereal::make_nvp("ColliderVertices", m_colliderVertices),
				cereal::make_nvp("ThrusterLocations", m_thrusterLocations));
	}

	void Copy(ShipStats* other);
	static ShipStats* Clone(ShipStats* other);

	static std::string GetTypeName() { static const std::string name = "ship"; return name; }
};

inline void ShipStats::Copy(ShipStats* other)
{
	m_maxHull = other->m_maxHull;

	m_interceptLeadMultiplier = other->m_interceptLeadMultiplier;
	m_followDistance = other->m_followDistance;
	m_approachDistance = other->m_approachDistance;
	m_strafeDistance = other->m_strafeDistance;
	m_sensorRange = other->m_sensorRange;

	m_shipThrust.Forward = other->m_shipThrust.Forward;
	m_shipThrust.Side = other->m_shipThrust.Side;
	m_shipThrust.Reverse = other->m_shipThrust.Reverse;
	m_shipThrust.Steer = other->m_shipThrust.Steer;

	m_dirGunData.hardPoints = { other->m_dirGunData.hardPoints };
	m_dirGunData.rigs = { other->m_dirGunData.rigs };

	m_misLauncherData.hardPoints = { other->m_misLauncherData.hardPoints };
	m_misLauncherData.rigs = { other->m_misLauncherData.rigs };

	m_shieldData.FrontStrength = other->m_shieldData.FrontStrength;
	m_shieldData.SideStrength = other->m_shieldData.SideStrength;
	m_shieldData.RearStrength = other->m_shieldData.RearStrength;
	m_shieldData.RegenSpeed = other->m_shieldData.RegenSpeed;

	m_colliderVertices = { other->m_colliderVertices };
	m_thrusterLocations = { other->m_thrusterLocations };
}

inline ShipStats* ShipStats::Clone(ShipStats* other)
{
	return new ShipStats(other->m_maxHull,
						other->m_interceptLeadMultiplier,
						other->m_followDistance,
						other->m_approachDistance,
						other->m_strafeDistance,
						other->m_sensorRange,
						ShipThrust(other->m_shipThrust),
						DirectionalGunData(other->m_dirGunData),
						MissileLauncherData(other->m_misLauncherData),
						ShieldData(other->m_shieldData),
						other->m_imageLocation,
						{other->m_colliderVertices},
						{other->m_thrusterLocations});
}