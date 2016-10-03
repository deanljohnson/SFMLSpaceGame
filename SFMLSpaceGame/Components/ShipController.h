#pragma once
#include "Component.h"
#include "ShipThrusters.h"
#include "DirectionalGun.h"

enum Maneuvers
{
	Follow, Intercept, Approach, FaceTargetAndFireDirectionalGuns, COUNT
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
	DirectionalGun* m_dirGuns{ nullptr };

	ManeuverBitset m_activeBehaviours;

	float m_interceptLeadMultiplier;
	float m_followDistance;
	float m_approachDistance;

	void FollowTarget();
	void InterceptTarget();
	void ApproachTarget();
	void FaceTargetAndFireDirectionalGuns();

public:
	ShipController(float interceptLead, float followDistance, float approachDistance)
		: m_interceptLeadMultiplier(interceptLead),
		  m_followDistance(followDistance),
		  m_approachDistance(approachDistance)
	{}

	virtual void Init() override;
	virtual void Update() override;

	void Set(Maneuvers maneuver, bool val = true);
	void SetTarget(Entity* target);
};