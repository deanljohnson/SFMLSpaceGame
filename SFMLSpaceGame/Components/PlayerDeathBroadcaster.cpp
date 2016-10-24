#include <Components/PlayerDeathBroadcaster.h>
#include <GameState.h>

void PlayerDeathBroadcaster::OnDestroy()
{
	Event playerDead = Event();
	playerDead.playerDied = Event::PlayerDiedEvent();
	playerDead.type = EventType::PlayerDied;

	GameState::pendingGameEvents.Push(playerDead);
}