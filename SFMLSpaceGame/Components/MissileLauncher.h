#pragma once
#include <HardPoint.h>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp> // Needed to serialize vector of hard points
#include <MissileRig.h>
#include <DefaultSerializeable.h>

struct MissileLauncherData
{
	std::vector<HardPoint> hardPoints;
	std::vector<std::shared_ptr<MissileRig>> rigs;

	// Loads the missile rig identified by the given name
	// to the given index in the MissileRig vector
	void LoadNewMissileRig(const std::string& rigName, size_t index);

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::make_nvp("HardPoints", hardPoints));

		std::vector<std::string> rigNames;
		for (auto& rig : rigs)
		{
			if (rig == nullptr)
				rigNames.push_back("empty");
			else
				rigNames.push_back(rig->name);
		}

		ar(cereal::make_nvp("MissileRigs", rigNames));
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::make_nvp("HardPoints", hardPoints));

		std::vector<std::string> rigNames;
		ar(cereal::make_nvp("MissileRigs", rigNames));

		rigs.resize(rigNames.size());

		for (auto i = 0; i < rigNames.size(); i++)
		{
			if (rigNames[i] == "empty")
				rigs[i] = nullptr;
			else
				rigs[i] = LoadRig<MissileRig>(rigNames[i]);
		}
	}
};

class MissileLauncher : public Component, public Gun, public DefaultSerializeable<MissileLauncher>
{
private:
	Position& m_position;
	Rotation& m_rotation;
	Sprite& m_sprite;
	SoundSource* m_shotSound{ nullptr };
	MissileLauncherData* m_launcherData;
	std::vector<float> m_lastFiringTimes;

	EntityID GetTarget(const b2Vec2& firingPoint);

	void FireWeapon(int i, EntityID target);
public:
	explicit MissileLauncher(EntityID ent);

	virtual void Shoot(const b2Vec2& pos = b2Vec2()) override;
	virtual float GetNormalizedHeat() override;

	void SetSoundSource(SoundSource* source);
	void SetLauncherData(MissileLauncherData* data);
};