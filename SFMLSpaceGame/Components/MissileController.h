#pragma once
class MissilePhysics;

class MissileController : public Component 
{
private:
	const float STEER_FORCE;

	MissilePhysics& m_physics;
	EntityID m_sourceEntity;
	EntityID m_targetEntity;

	float m_currentTorque;
	float m_thrust;

	EntityHandle m_targetHandle;

	void SteerLeft(float amount);
	void SteerRight(float amount);

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_sourceEntity, m_targetEntity, m_currentTorque, m_thrust);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<MissileController>& construct)
	{
		EntityID selfID;
		EntityID sourceID;
		EntityID targetID;
		float thrust;
		ar(selfID, sourceID, targetID, thrust);
		construct(selfID, sourceID, targetID, thrust);

		ar(construct->m_currentTorque);
	}
public:
	MissileController(EntityID ent, EntityID sourceEnt, EntityID target, float thrust);

	virtual void Update() override;
};