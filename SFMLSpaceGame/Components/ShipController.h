#pragma once
#include "Component.h"
#include "ShipThrusters.h"
#include "DirectionalGun.h"
#include <ShipStats.h>
#include <EntityHandle.h>

enum Maneuvers
{
	Follow, 
	Intercept, 
	Approach, 
	FireGuns,
	FireGunsWhenFacingTarget,
	FaceTargetForAttack, 
	StrafeLeftForAttack, 
	StrafeRightForAttack, 
	StrafeToTargetsRearForAttack,
	COUNT
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

	std::shared_ptr<ShipStats> m_stats;

	void FollowTarget();
	void InterceptTarget();
	void ApproachTarget();
	void FireGuns();
	void FireGunsWhenFacingTarget();
	void FaceTargetForAttack();
	void StrafeForAttack(ThrustDirection dir);
	void StrafeToRearForAttack();

public:
	ShipController(std::shared_ptr<ShipStats> stats)
		: m_stats(stats)
	{}

	virtual void Init() override;
	virtual void Update() override;

	void Set(Maneuvers maneuver, bool val = true);
	void SetTarget(EntityHandle& target);
};