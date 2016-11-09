#pragma once
#include "Component.h"

class PlayerDeathBroadcaster : public Component
{
public:
	explicit PlayerDeathBroadcaster(EntityID ent);
	virtual void OnDestroy() override;
};