#pragma once
#include "GameWindow.h"
#include <Entity.h>

class StationWindow : public GameWindow
{
private:
	EntityID m_target;
public:
	StationWindow();

	void SetTarget(EntityID targetID);
};