#pragma once
#include "GameWindow.h"

class StationWindow : public GameWindow
{
private:
	EntityID m_target;
public:
	StationWindow();

	void SetTarget(EntityID targetID);
};