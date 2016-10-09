#pragma once
#include "Component.h"

class PlayerDeathBroadcaster : public Component
{
public:
	virtual void OnDestroy() override;
};