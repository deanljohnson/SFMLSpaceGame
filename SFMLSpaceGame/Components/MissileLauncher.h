#pragma once
#include <Components\Component.h>
#include <HardPoint.h>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp> // Needed to serialize vector of hard points

struct MissileLauncherData
{
	MissileLauncherData()
		: fireRate(0.f)
	{}

	explicit MissileLauncherData(float fr, const std::initializer_list<HardPoint>& hardPointLocations)
		: fireRate(fr)
	{}

	float fireRate;
	std::vector<HardPoint> hardPoints;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("FireRate", fireRate),
				cereal::make_nvp("HardPoints", hardPoints));
	}
};

class MissileLauncher : public Component, public Gun
{
private:
	Position& m_position;
	Rotation& m_rotation;
	Sprite& m_sprite;
	SoundSource* m_shotSound{ nullptr };
	float m_lastFiringTime;
	MissileLauncherData* m_launcherData;

	EntityID GetTarget();

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_lastFiringTime);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<MissileLauncher>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		ar(construct->m_lastFiringTime);
	}
public:
	explicit MissileLauncher(EntityID ent);

	virtual void Shoot() override;
	virtual float GetNormalizedHeat() override;

	void SetSoundSource(SoundSource* source);
	void SetLauncherData(MissileLauncherData* data);
};