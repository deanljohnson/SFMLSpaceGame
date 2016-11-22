#include "stdafx.h"
#include <Components/PlayerDeathBroadcaster.h>
#include <GameState.h>

PlayerDeathBroadcaster::PlayerDeathBroadcaster(EntityID ent)
	: Component(ent)
{
}

void PlayerDeathBroadcaster::OnDestroy()
{
	Event playerDead{};
	playerDead.playerDied = Event::PlayerDiedEvent();
	playerDead.type = EventType::PlayerDied;

	GameState::pendingGameEvents.Push(playerDead);
}