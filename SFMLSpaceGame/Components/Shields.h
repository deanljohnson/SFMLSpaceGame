#pragma once
#include <cereal/cereal.hpp>
#include <Components\Position.h>
#include <Components\Rotation.h>
#include <Interfaces\AttackEventModifier.h>

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

	void AbsorbFrontDamage(Event& event);
	void AbsorbSideDamage(Event& event);
	void AbsorbRearDamage(Event& event);

	std::function<void(Direction, const b2Vec2&)> m_shieldHitCallback{nullptr};

public:
	Shields(EntityID ent, ShieldData* data);

	virtual void Update() override;

	void SetActive(Direction dir);
	void Modify(Event& event) override;

	void SetShieldHitCallback(std::function<void(Direction, const b2Vec2&)> callback);

	float GetMaxStrength(Direction dir);
	float GetCurrentStrength(Direction dir);
};