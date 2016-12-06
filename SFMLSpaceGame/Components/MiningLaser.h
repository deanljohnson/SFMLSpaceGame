#pragma once
#include <Components/Component.h>
#include <Interfaces/Gun.h>
#include <MiningRig.h>
#include <HardPoint.h>

class Position;
class Rotation;
class SoundSource;

struct MiningLaserData
{
	HardPoint hardPoint;
	MiningRig rig;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::make_nvp("HardPoint", hardPoint),
			cereal::make_nvp("Rig", rig));
	}
};

class MiningLaser : public Component, public Gun
{
private:
	Position& m_position;
	Rotation& m_rotation;
	SoundSource* m_shotSound{ nullptr };
	MiningLaserData* m_laserData;

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
public:
	explicit MiningLaser(EntityID ent);

	virtual void Shoot(const b2Vec2& pos = b2Vec2()) override;
	virtual float GetNormalizedHeat() override;

	void SetSoundSource(SoundSource* source);
	void SetLaserData(MiningLaserData* data);
};