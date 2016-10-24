#include "ParallaxTargetAssigner.h"
#include <Entity.h>
#include <GameState.h>
#include <EntityManager.h>

void ParallaxTargetAssigner::Init()
{
	m_parallax = &entity->GetComponent<ParallaxMovement>();
}

void ParallaxTargetAssigner::Update()
{
	if (m_parallax->HasValidTarget())
		return;

	Event event;
	if (GameState::pendingGameEvents.Get(EventType::PlayerSpawned, event))
	{
		m_parallax->SetTarget(EntityManager::Get(event.playerSpawned.ID));
	}
}