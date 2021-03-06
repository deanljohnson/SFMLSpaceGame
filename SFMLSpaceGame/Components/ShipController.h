#pragma once

class ShipStats;
class DirectionalGun;
class ShipThrusters;

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
class ShipController : public Component, public Updateable
{
private:
	Physics& m_physics;
	ShipThrusters& m_thrusters;
	DirectionalGun& m_dirGuns;

	Physics* m_target{ nullptr };
	EntityID m_targetID;

	ManeuverBitset m_activeBehaviours;

	std::string m_statsID;
	std::shared_ptr<ShipStats> m_stats;

	void FollowTarget();
	void InterceptTarget();
	void ApproachTarget();
	void FireGuns();
	void FireGunsWhenFacingTarget();
	void FaceTargetForAttack();
	void StrafeForAttack(ThrustDirection dir);
	void StrafeToRearForAttack();

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(GetEntityID(), m_statsID, m_targetID);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<ShipController>& construct)
	{
		EntityID selfID;
		std::string statsID;
		EntityID targetID;
		ar(selfID, statsID, targetID);
		construct(selfID, statsID);

		construct->SetTarget(targetID);
	}

public:
	ShipController(EntityID ent, const std::string& shipStatsID);

	virtual void Update() override;

	void SetThrusterPower(float val);

	void Set(Maneuvers maneuver, bool val = true);
	void Clear();
	void SetTarget(EntityID target);
};