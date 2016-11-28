#pragma once
#include <Event.h>
#include <EntityManager.h>

class ShipController;

class ShipAI : public Component
{
private:
	enum class AIState{ None, AttackingShip, MovingToStation } m_currentState { AIState::None };

	EntityID m_lastStationReached{ ENTITY_ID_NULL };
	Position* m_stationPosition;

	std::string m_shipStatsID;
	std::shared_ptr<ShipStats> m_shipStats;
	ShipController& m_controller;
	Position& m_position;
	EntityHandle m_targetHandle;

	void ProcessEvents();
	void ProcessAIState();
	void HandleAttackedEvent(Event::AttackedEvent event);

	void FindStation();

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_shipStatsID, m_currentState, m_lastStationReached, m_targetHandle.GetID());
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<ShipAI>& construct)
	{
		EntityID selfID;
		std::string shipStatsID;
		ar(selfID, shipStatsID);
		construct(selfID, shipStatsID);

		EntityID targetID;
		ar(construct->m_currentState,
			construct->m_lastStationReached,
			targetID);
		construct->m_targetHandle = EntityManager::Get(targetID);
	}
public:
	explicit ShipAI(EntityID ent, const std::string& shipID);
	virtual void Update() override;
};