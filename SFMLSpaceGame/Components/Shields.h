#pragma once
#include <Components\Position.h>
#include <Components\Rotation.h>
#include <Interfaces\AttackEventModifier.h>

struct ShieldData 
{
	ShieldData()
		: FrontStrength(0), SideStrength(0), RearStrength(0)
	{}
	ShieldData(float front, float side, float rear)
		: FrontStrength(front),
		  SideStrength(side),
		  RearStrength(rear)
	{}

	float FrontStrength, SideStrength, RearStrength;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("FrontStrength", FrontStrength),
				cereal::make_nvp("SideStrength", SideStrength),
				cereal::make_nvp("RearStrength", RearStrength));
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
	const float MAX_SHIELD_STRENGTH = 100.f;

	Position* m_position;
	Rotation* m_rotation;
	Direction m_activationMask;
	ShieldData* m_data;

	void ModifyWithAmount(Event& event, float amount) const;
	void ModifyForFrontHit(Event& event) const;
	void ModifyForSideHit(Event& event) const;
	void ModifyForRearHit(Event& event) const;
public:

	explicit Shields(ShieldData* data)
		: m_position(nullptr),
		  m_rotation(nullptr),
		  m_activationMask(Direction::All),
		  m_data(data)
	{}

	virtual void Init() override;

	void SetActive(Direction dirMask);
	void Modify(Event& event);
};