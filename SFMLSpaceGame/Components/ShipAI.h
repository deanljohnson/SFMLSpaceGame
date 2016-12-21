#pragma once
#include <EntityID.h>

class ShipController;
class AttackedEvent;

class ShipAI : public Component, public Updateable
{
private:
	enum class AIState{ None, AttackingShip, MovingToStation } m_currentState { AIState::None };

	EntityID m_lastStationReached{ ENTITY_ID_NULL };
	Position* m_stationPosition;

	std::string m_shipStatsID;
	std::shared_ptr<ShipStats> m_shipStats;
	ShipController& m_controller;
	Position& m_position;
	EntityID m_targetID;

	void ProcessEvents();
	void ProcessAIState();
	void HandleAttackedEvent(AttackedEvent* event);

	void FindTrade();

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_shipStatsID, m_currentState, m_lastStationReached, m_targetID);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<ShipAI>& construct)
	{
		EntityID selfID;
		std::string shipStatsID;
		ar(selfID, shipStatsID);
		construct(selfID, shipStatsID);

		ar(construct->m_currentState,
			construct->m_lastStationReached,
			construct->m_targetID);
	}
public:
	explicit ShipAI(EntityID ent, const std::string& shipID);
	virtual void Update() override;
};