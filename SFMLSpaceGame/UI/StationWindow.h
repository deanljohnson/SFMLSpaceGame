#pragma once
#include "GameWindow.h"

class StationWindow : public GameWindow
{
private:
	struct StationWindowImpl;

	EntityID m_target;
	std::unique_ptr<StationWindowImpl> m_impl;
public:
	StationWindow();
	~StationWindow();

	void SetTarget(EntityID targetID);
};