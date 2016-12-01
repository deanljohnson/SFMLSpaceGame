#include "stdafx.h"
#include "ParallaxTargetAssigner.h"
#include "ParallaxMovement.h"
#include <Entity.h>
#include <GameState.h>
#include <EntityManager.h>

ParallaxTargetAssigner::ParallaxTargetAssigner(EntityID ent)
	: Component(ent),
	  m_parallax(entity->GetComponent<ParallaxMovement>())
{}

void ParallaxTargetAssigner::Update()
{
	if (m_parallax.HasValidTarget())
		return;

	PlayerSpawnedEvent* event{nullptr};
	if (event = GameState::pendingGameEvents.Get<EventType::PlayerSpawned>())
	{
		m_parallax.SetTarget(EntityManager::Get(event->ID));
	}
}