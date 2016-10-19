#pragma once
#include <UI/GameWindow.h>
#include <EntityHandle.h>
#include <Components\Shields.h>
#include <SFGUI\ProgressBar.hpp>

class ShieldStateDisplay : public GameWindow 
{
private:
	EntityHandle m_targetHandle;
	Shields* m_targetShields;

	sfg::ProgressBar::Ptr m_frontState;
	sfg::ProgressBar::Ptr m_sideState;
	sfg::ProgressBar::Ptr m_rearState;
public:
	ShieldStateDisplay();

	virtual void Update() override;

	void SetTarget(const EntityID& id);
};