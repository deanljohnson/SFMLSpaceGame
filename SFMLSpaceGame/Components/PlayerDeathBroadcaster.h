#pragma once
#include <DefaultSerializeable.h>

class PlayerDeathBroadcaster : public Component, public DefaultSerializeable<PlayerDeathBroadcaster>
{
public:
	explicit PlayerDeathBroadcaster(EntityID ent);
	virtual void OnDestroy() override;
};