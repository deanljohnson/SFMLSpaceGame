#pragma once
#include <Components/ShipThrusters.h>
#include <Components/DirectionalGun.h>

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

public:
	ShipStats() 
		: m_interceptLeadMultiplier(0),
		  m_followDistance(0),
		  m_approachDistance(0),
		  m_strafeDistance(0),
		  m_sensorRange(0),
		  m_shipThrust(),
		  m_dirGunData()
	{}

	ShipStats(const ShipStats& other)
		: m_interceptLeadMultiplier{other.m_interceptLeadMultiplier},
		  m_followDistance{other.m_followDistance},
		  m_approachDistance{other.m_approachDistance},
		  m_strafeDistance{other.m_strafeDistance},
		  m_sensorRange{other.m_sensorRange},
		  m_shipThrust{other.m_shipThrust},
		  m_dirGunData{other.m_dirGunData},
		  m_imageLocation{other.m_imageLocation}
	{
	}

	ShipStats(float interceptLead, float followDistance, 
				float approachDistance, float strafeDistance, 
				float sensorRange, ShipThrust thrust, 
				DirectionalGunData dirData, std::string imageLocation)
		: m_interceptLeadMultiplier(interceptLead),
		  m_followDistance(followDistance),
		  m_approachDistance(approachDistance),
		  m_strafeDistance(strafeDistance),
		  m_sensorRange(sensorRange),
		  m_shipThrust(thrust),
		  m_dirGunData(dirData),
		  m_imageLocation(imageLocation)
	{}

	inline float GetInterceptLeadMultiplier() const { return m_interceptLeadMultiplier; }
	inline float GetFollowDistance() const { return m_followDistance; }
	inline float GetApproachDistance() const { return m_approachDistance; }
	inline float GetStrafeDistance() const { return m_strafeDistance; }
	inline float GetSensorRange() const { return m_sensorRange; }
	inline ShipThrust* GetShipThrust() { return &m_shipThrust; }
	inline DirectionalGunData* GetDirGunData() { return &m_dirGunData; }
	inline std::string GetImageLocation() { return m_imageLocation; }

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
				cereal::make_nvp("ImageLocation", m_imageLocation));
	}

	static std::string GetTypeName() { return "ship"; }
};