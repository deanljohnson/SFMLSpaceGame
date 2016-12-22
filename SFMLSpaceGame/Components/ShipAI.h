#pragma once
#include <EntityID.h>

class ShipController;
class AttackedEvent;
class FindBestPurchaseJob;
class FindBestSaleJob;
struct FindPurchaseJobResult;

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

	std::unique_ptr<FindPurchaseJobResult> m_purchaseResult;

	std::shared_ptr<FindBestPurchaseJob> m_findPurchaseJob;
	std::shared_ptr<FindBestSaleJob> m_findSaleJob;

	void ProcessEvents();
	void ProcessAIState();
	void HandleAttackedEvent(AttackedEvent* event);

	void FindTrade();
	void CreatePurchaseJob();
	void CreateSaleJob();

	void ProcessFinishedPurchaseJob();
	void ProcessFinishedSaleJob();

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