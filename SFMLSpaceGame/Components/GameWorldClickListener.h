#pragma once
#include <Components/Component.h>

class GameWorldClickListener : public Component
{
private:
	enum ClickState { None, Down, Held, Click } m_clickedState;
public:
	virtual void Update() override;
};