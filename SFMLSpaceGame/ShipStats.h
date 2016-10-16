#pragma once
#include <Components/ShipThrusters.h>
#include <Components/DirectionalGun.h>
#include <cereal/types/vector.hpp> // Need for collider vertices serialization
#include <SFMLSerialization.h> // Need for collider vertices serialization

class ShipStats
{
private:
	float m_interceptLeadMultiplier;
	float m_followDistance;
	float m_approachDistance;
	float m_strafeDistance;
	float m_sensorRange;
	ShipThrust m_shipThrust;
	DirectionalGunData m_dirGunData;
	std::string m_imageLocation;
	std::vector<sf::Vector2f> m_colliderVertices;

public:
	ShipStats() 
		: m_interceptLeadMultiplier(0),
		  m_followDistance(0),
		  m_approachDistance(0),
		  m_strafeDistance(0),
		  m_sensorRange(0),
		  m_shipThrust(),
		  m_dirGunData(),
		m_colliderVertices()
	{}

	ShipStats(const ShipStats& other)
		: m_interceptLeadMultiplier{other.m_interceptLeadMultiplier},
		  m_followDistance{other.m_followDistance},
		  m_approachDistance{other.m_approachDistance},
		  m_strafeDistance{other.m_strafeDistance},
		  m_sensorRange{other.m_sensorRange},
		  m_shipThrust{other.m_shipThrust},
		  m_dirGunData{other.m_dirGunData},
		  m_imageLocation{other.m_imageLocation},
		  m_colliderVertices{other.m_colliderVertices}
	{}

	ShipStats(float interceptLead, float followDistance, 
				float approachDistance, float strafeDistance, 
				float sensorRange, ShipThrust thrust, 
				DirectionalGunData dirData, std::string imageLocation,
				const std::vector<sf::Vector2f>& colliderVertices)
		: m_interceptLeadMultiplier(interceptLead),
		  m_followDistance(followDistance),
		  m_approachDistance(approachDistance),
		  m_strafeDistance(strafeDistance),
		  m_sensorRange(sensorRange),
		  m_shipThrust(thrust),
		  m_dirGunData(dirData),
		  m_imageLocation(imageLocation),
		  m_colliderVertices(colliderVertices)
	{}

	inline float GetInterceptLeadMultiplier() const { return m_interceptLeadMultiplier; }
	inline float GetFollowDistance() const { return m_followDistance; }
	inline float GetApproachDistance() const { return m_approachDistance; }
	inline float GetStrafeDistance() const { return m_strafeDistance; }
	inline float GetSensorRange() const { return m_sensorRange; }
	inline ShipThrust* GetShipThrust() { return &m_shipThrust; }
	inline DirectionalGunData* GetDirGunData() { return &m_dirGunData; }
	inline std::string GetImageLocation() { return m_imageLocation; }
	inline std::vector<sf::Vector2f>& GetColliderVertices() { return m_colliderVertices; }

	inline void SetInterceptLeadMultiplier(float val) { m_interceptLeadMultiplier = val; }
	inline void SetFollowDistance(float val) { m_followDistance = val; }
	inline void SetApproachDistance(float val) { m_approachDistance = val; }
	inline void SetStrafeDistance(float val) { m_strafeDistance = val; }
	inline void SetSensorRange(float val) { m_sensorRange = val; }
	inline void SetImageLocation(const std::string& val) { m_imageLocation = val; }
	inline void SetColliderVertices(const std::vector<sf::Vector2f>& colliderVertices) { m_colliderVertices = { colliderVertices }; }

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("InterLeadMult", m_interceptLeadMultiplier),
				cereal::make_nvp("FollowDist", m_followDistance),
				cereal::make_nvp("ApproachDist", m_approachDistance),
				cereal::make_nvp("StrafeDist", m_strafeDistance),
				cereal::make_nvp("SensorRange", m_sensorRange),
				cereal::make_nvp("ShipThrust", m_shipThrust),
				cereal::make_nvp("DirGunData", m_dirGunData),
				cereal::make_nvp("ImageLocation", m_imageLocation),
				cereal::make_nvp("ColliderVertices", m_colliderVertices));
	}

	void Copy(ShipStats* other);

	static std::string GetTypeName() { return "ship"; }
	static ShipStats* Clone(ShipStats* other);
};

inline void ShipStats::Copy(ShipStats* other)
{
	m_interceptLeadMultiplier = other->m_interceptLeadMultiplier;
	m_followDistance = other->m_followDistance;
	m_approachDistance = other->m_approachDistance;
	m_strafeDistance = other->m_strafeDistance;
	m_sensorRange = other->m_sensorRange;

	m_shipThrust.Forward = other->m_shipThrust.Forward;
	m_shipThrust.Side = other->m_shipThrust.Side;
	m_shipThrust.Reverse = other->m_shipThrust.Reverse;
	m_shipThrust.Steer = other->m_shipThrust.Steer;

	m_dirGunData.fireRate = other->m_dirGunData.fireRate;
	m_dirGunData.heatLimit = other->m_dirGunData.heatLimit;
	m_dirGunData.cooldownRate = other->m_dirGunData.cooldownRate;
	m_dirGunData.heatGenerated = other->m_dirGunData.heatGenerated;

	m_colliderVertices = { other->m_colliderVertices };
}

inline ShipStats* ShipStats::Clone(ShipStats* other)
{
	return new ShipStats(other->m_interceptLeadMultiplier,
						other->m_followDistance,
						other->m_approachDistance,
						other->m_strafeDistance,
						other->m_sensorRange,
						ShipThrust(other->m_shipThrust),
						DirectionalGunData(other->m_dirGunData),
						other->m_imageLocation,
						{other->m_colliderVertices});
}