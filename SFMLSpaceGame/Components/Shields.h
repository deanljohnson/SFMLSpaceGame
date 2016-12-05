#pragma once
#include <cereal/cereal.hpp>
#include <Interfaces/AttackEventModifier.h>

class Position;
class Rotation;

struct ShieldData 
{
	ShieldData()
		: FrontStrength(0), SideStrength(0), RearStrength(0), RegenSpeed(0)
	{}
	ShieldData(float front, float side, float rear, float regen)
		: FrontStrength(front),
		  SideStrength(side),
		  RearStrength(rear),
		  RegenSpeed(regen)
	{}

	float FrontStrength, SideStrength, RearStrength;
	float RegenSpeed;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("FrontStrength", FrontStrength),
				cereal::make_nvp("SideStrength", SideStrength),
				cereal::make_nvp("RearStrength", RearStrength),
			    cereal::make_nvp("RegenSpeed", RegenSpeed));
	}
};

class Shields : public Component, public AttackedEventModifier
{
public:
	enum class Direction : int
	{
		Front = 0x1,
		Side = 0x2,
		Rear = 0x4,
		All = 0x7 
	};
private:
	Position& m_position;
	Rotation& m_rotation;
	Direction m_activationMask;
	ShieldData* m_data;

	float m_currentFrontStrength;
	float m_currentSideStrength;
	float m_currentRearStrength;

	void OnShieldDeplete();

	void AbsorbFrontDamage(AttackedEvent* event);
	void AbsorbSideDamage(AttackedEvent* event);
	void AbsorbRearDamage(AttackedEvent* event);

	std::function<void(Direction, const b2Vec2&)> m_shieldHitCallback{nullptr};

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_activationMask, m_currentFrontStrength, m_currentSideStrength, m_currentRearStrength);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Shields>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		ar(construct->m_activationMask, 
			construct->m_currentFrontStrength, 
			construct->m_currentSideStrength, 
			construct->m_currentRearStrength);
	}

public:
	explicit Shields(EntityID ent);

	virtual void Update() override;

	void SetActive(Direction dir);
	void Modify(AttackedEvent* event) override;

	void SetShieldHitCallback(std::function<void(Direction, const b2Vec2&)> callback);
	void SetShieldData(ShieldData* data);

	float GetMaxStrength(Direction dir);
	float GetCurrentStrength(Direction dir);
};