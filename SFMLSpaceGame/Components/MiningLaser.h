#pragma once
#include <Interfaces/Gun.h>
#include <MiningRig.h>
#include <HardPoint.h>

class Position;
class Rotation;
class SoundSource;
struct BatchIndex;

struct MiningLaserData
{
	HardPoint hardPoint;
	std::shared_ptr<MiningRig> rig;

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::make_nvp("HardPoint", hardPoint));

		if (rig == nullptr)
			ar(cereal::make_nvp("Rig", "empty"));
		else ar(cereal::make_nvp("Rig", rig->name));
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::make_nvp("HardPoint", hardPoint));

		std::string rigName;
		ar(cereal::make_nvp("Rig", rigName));

		if (rigName == "empty")
			rig == nullptr;
		else rig = LoadRig<MiningRig>(rigName);
	}
};

class MiningLaser : public Component, public Gun
{
private:
	Position& m_position;
	Rotation& m_rotation;
	SoundSource* m_shotSound{ nullptr };
	MiningLaserData* m_laserData;
	BatchIndex* m_batchIndex;

	float m_miningTimer;
	float m_laserTexAnimOffset; // The texCoords offset for the laser vertices
	float m_laserTexAnimRate; // The speed at which the laser texture is scrolled
	float m_miningLaserThickness; // The thickness, in meters, of the laser
	bool m_firing; // Tracks whether we are firing in each frame
	EntityID m_targetID; // The ID of the asteroid we are targeting. ENTITY_ID_NULL if none
	EntityHandle m_targetHandle; // The handle of our target. Check m_targetID for null to make sure this is still accurate

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(GetEntityID(), GetSoundSourceID());
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<MiningLaser>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		int soundSourceID;
		ar(soundSourceID);

		construct->InitSoundSource(soundSourceID);
	}

	// Defined only to move implementation into cpp file
	int GetSoundSourceID();
	// Defined only to move implementation into cpp file
	void InitSoundSource(int compID);

	void HideLaser();
	void ShowLaser();
	void DoMining();
public:
	explicit MiningLaser(EntityID ent);

	virtual void Update() override;

	virtual void Shoot(const b2Vec2& pos = b2Vec2()) override;
	virtual float GetNormalizedHeat() override;

	void SetSoundSource(SoundSource* source);
	void SetLaserData(MiningLaserData* data);
};