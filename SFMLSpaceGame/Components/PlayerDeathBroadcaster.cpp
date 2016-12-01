#include "stdafx.h"
#include <Components/PlayerDeathBroadcaster.h>
#include <GameState.h>

PlayerDeathBroadcaster::PlayerDeathBroadcaster(EntityID ent)
	: Component(ent)
{
}

void PlayerDeathBroadcaster::OnDestroy()
{
	std::unique_ptr<PlayerDiedEvent> playerDied
	{
		new PlayerDiedEvent()
	};
	GameState::pendingGameEvents.Push(move(playerDied));
}