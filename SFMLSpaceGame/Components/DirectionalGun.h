#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Components/Sprite.h>
#include <Components/SoundSource.h>
#include <Interfaces/Gun.h>
#include <vector>
#include <ResourceLoader.h>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp> // Needed to serialize vector of hard points
#include <HardPoint.h>
#include <Entity.h>
#include <LaserRig.h>

struct DirectionalGunData
{
	DirectionalGunData() 
		: fireRate(0),
		  heatLimit(0),
		  cooldownRate(0),
		  heatGenerated(0),
		  hardPoints(),
		  soundID(0)
	{}
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
	std::vector<std::shared_ptr<LaserRig>> rigs;
	ResourceID soundID;

	template<class Archive>
	void save(Archive& ar) const 
	{
		ar(cereal::make_nvp("FireRate", fireRate),
			cereal::make_nvp("HealLimit", heatLimit),
			cereal::make_nvp("CooldownRate", cooldownRate),
			cereal::make_nvp("HeatGenerated", heatGenerated),
			cereal::make_nvp("HardPoints", hardPoints),
			cereal::make_nvp("SoundID", soundID));

		// save the number of rigs being serialized
		// so we know how many to load later
		ar(cereal::make_nvp("NumRigs", rigs.size());
		for (auto& rig : rigs) 
		{
			ar(rig->name);
		}
	}

	template<class Archive>
	void load(Archive& ar) 
	{
		ar(cereal::make_nvp("FireRate", fireRate),
			cereal::make_nvp("HealLimit", heatLimit),
			cereal::make_nvp("CooldownRate", cooldownRate),
			cereal::make_nvp("HeatGenerated", heatGenerated),
			cereal::make_nvp("HardPoints", hardPoints),
			cereal::make_nvp("SoundID", soundID));

		// save the number of rigs being serialized
		// so we know how many to load later
		int numRigs;
		ar(cereal::make_nvp("NumRigs", numRigs);
		for (auto& rig : rigs)
		{
			ar(rig->name);
		}
	}

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::make_nvp("FireRate", fireRate),
				cereal::make_nvp("HealLimit", heatLimit),
				cereal::make_nvp("CooldownRate", cooldownRate),
				cereal::make_nvp("HeatGenerated", heatGenerated),
				cereal::make_nvp("HardPoints", hardPoints),
				cereal::make_nvp("Rigs", rigs),
				cereal::make_nvp("SoundID", soundID));
	}
};

class DirectionalGun : public Component, public Gun
{
private:
	struct WeaponState 
	{
		float currentHeat;
		float lastFiringTime;
	};

	Position& m_position;
	Rotation& m_rotation;
	Sprite& m_sprite;
	SoundSource* m_shotSound{ nullptr };
	float m_lastFiringTime;
	DirectionalGunData* m_gunData;
	float m_currentHeat{ 0.f };
	std::vector<WeaponState> m_weaponStates;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), 	(m_shotSound == nullptr) 
								? -1 
								: entity->GetComponentID(*m_shotSound));
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<DirectionalGun>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		int soundSourceID;
		ar(soundSourceID);

		if (soundSourceID != -1)
			construct->m_shotSound = &construct->entity->GetComponent<SoundSource>(soundSourceID);
	}

	void FireWeapon(int i);
public:
	explicit DirectionalGun(EntityID ent);

	virtual void Update() override;

	virtual void Shoot(const b2Vec2& pos = b2Vec2()) override;
	virtual float GetNormalizedHeat() override;

	void SetSoundSource(SoundSource* source);
	void SetGunData(DirectionalGunData* data);
};