#pragma once
#include "Component.h"
#include "ShipThrusters.h"

enum Maneuvers
{
	Follow, Intercept, Approach, COUNT
};

using ManeuverBitset = std::bitset<COUNT>;

// Encapsulates various high-level ship maneuvers in a single interface.
// Multiple behaviours may be combined to create more complicated behaviours
class ShipController : public Component
{
private:
	Physics* m_physics{ nullptr };
	ShipThrusters* m_thrusters{ nullptr };
	Physics* m_target{ nullptr };

	float m_interceptLeadMultiplier;
	float m_followDistance;
	float m_approachDistance;

	void FollowTarget();
	void InterceptTarget();
	void ApproachTarget();

public:
	ShipController(float interceptLead, float followDistance, float approachDistance)
		: m_interceptLeadMultiplier(interceptLead),
		  m_followDistance(followDistance),
		  m_approachDistance(approachDistance)
	{}

	ManeuverBitset activeBehaviours;

	virtual void Init() override;
	virtual void Update() override;

	void SetTarget(Entity* target);
};