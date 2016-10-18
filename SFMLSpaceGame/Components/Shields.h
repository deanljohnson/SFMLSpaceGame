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
	float RegenSpeed;

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
	Position* m_position;
	Rotation* m_rotation;
	Direction m_activationMask;
	ShieldData* m_data;

	float m_currentFrontStrength;
	float m_currentSideStrength;
	float m_currentRearStrength;

	void OnShieldDeplete();

	void AbsorbFrontDamage(Event& event);
	void AbsorbSideDamage(Event& event);
	void AbsorbRearDamage(Event& event);
public:

	explicit Shields(ShieldData* data)
		: m_position(nullptr),
		  m_rotation(nullptr),
		  m_activationMask(Direction::All),
		  m_data(data)
	{}

	virtual void Init() override;
	virtual void Update() override;

	void SetActive(Direction dirMask);
	void Modify(Event& event);
};