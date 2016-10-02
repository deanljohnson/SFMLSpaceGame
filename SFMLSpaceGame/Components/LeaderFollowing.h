#pragma once
#include "Physics.h"
#include "ShipThrusters.h"

class LeaderFollowing : public Component
{
private:
	Physics* m_physics{ nullptr };
	ShipThrusters* m_thrusters{ nullptr };
	Entity* m_leader{ nullptr };
public:
	virtual void Init() override;
	virtual void Update() override;

	void SetLeader(Entity* leader);
};