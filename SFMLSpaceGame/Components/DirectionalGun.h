#pragma once
#include <Interfaces/Gun.h>
#include <vector>
#include <ResourceLoader.h>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp> // Needed to serialize vector of hard points
#include <HardPoint.h>

class Position;
class Rotation;
class Sprite;
class LaserRig;
class SoundSource;

struct DirectionalGunData
{
	std::vector<HardPoint> hardPoints;
	std::vector<std::shared_ptr<LaserRig>> rigs;
	ResourceID soundID{0};

	// Loads the laser rig identified by the given name
	// to the given index in the LaserRig array
	void LoadNewLaserRig(const std::string& rigName, size_t index);

	template<class Archive>
	void save(Archive& ar) const 
	{
		ar(cereal::make_nvp("HardPoints", hardPoints),
			cereal::make_nvp("SoundID", soundID));

		std::vector<std::string> rigNames;
		for (auto& rig : rigs)
		{
			if (rig == nullptr)
				rigNames.push_back("empty");
			else
				rigNames.push_back(rig->name);
		}

		ar(cereal::make_nvp("GunRigs", rigNames));
	}

	template<class Archive>
	void load(Archive& ar) 
	{
		ar(cereal::make_nvp("HardPoints", hardPoints),
			cereal::make_nvp("SoundID", soundID));

		std::vector<std::string> rigNames;
		ar(cereal::make_nvp("GunRigs", rigNames));

		rigs.resize(rigNames.size());

		for (int i = 0; i < rigNames.size(); i++)
		{
			if (rigNames[i] == "empty")
				rigs[i] = nullptr;
			else
				rigs[i] = LoadRig<LaserRig>(rigNames[i]);
		}
	}
};

class DirectionalGun : public Component, public Gun
{
private:
	struct WeaponState 
	{
		float currentHeat{0.f};
		float lastFiringTime{0.f};
	};

	Position& m_position;
	Rotation& m_rotation;
	Sprite& m_sprite;
	SoundSource* m_shotSound{ nullptr };
	DirectionalGunData* m_gunData;
	std::vector<WeaponState> m_weaponStates;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(GetEntityID(), GetSoundSourceID());
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<DirectionalGun>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		int soundSourceID;
		ar(soundSourceID);

		construct->InitSoundSource(soundSourceID);
	}

	void FireWeapon(int i);

	// Defined only to move implementation into cpp file
	int GetSoundSourceID();
	// Defined only to move implementation into cpp file
	void InitSoundSource(int compID);
public:
	explicit DirectionalGun(EntityID ent);

	virtual void Update() override;

	virtual void Shoot(const b2Vec2& pos = b2Vec2()) override;
	virtual float GetNormalizedHeat() override;

	void SetSoundSource(SoundSource* source);
	void SetGunData(DirectionalGunData* data);
};