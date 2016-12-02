// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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