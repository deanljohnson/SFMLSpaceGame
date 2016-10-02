#include <Components/LeaderFollowing.h>
#include <Entity.h>

void LeaderFollowing::Init()
{
	m_physics = &entity->GetComponent<Physics>();
	m_thrusters = &entity->GetComponent<ShipThrusters>();
}

void LeaderFollowing::Update()
{
	if (m_leader == nullptr)
		return;
}

void LeaderFollowing::SetLeader(Entity* leader)
{
	m_leader = leader;
}
