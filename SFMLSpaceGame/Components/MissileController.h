#pragma once
#include <Components\Component.h>
#include <Components\MissilePhysics.h>

class MissileController : public Component 
{
private:
	const float STEER_FORCE;

	MissilePhysics& m_physics;
	EntityID m_sourceEntity;
	EntityID m_targetEntity;

	std::shared_ptr<MissileStats> m_missStats;
	std::string m_projID;

	float m_currentTorque;

	EntityHandle m_targetHandle;

	void SteerLeft(float amount);
	void SteerRight(float amount);

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_sourceEntity, m_targetEntity, m_projID, m_currentTorque);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<MissileController>& construct)
	{
		EntityID selfID;
		EntityID sourceID;
		EntityID targetID;
		std::string projID;
		ar(selfID, sourceID, targetID, projID);
		construct(selfID, sourceID, targetID, projID);

		ar(construct->m_currentTorque);
	}
public:
	MissileController(EntityID ent, EntityID sourceEnt, EntityID target, const std::string& projID);

	virtual void Update() override;
};