#pragma once
#include <Components/Component.h>
#include <Interfaces/MouseListener.h>
#include <vector>

class GameWorldClickListener : public Component
{
private:
	enum ClickState { None, Down, Held, Click } m_clickedState;
	std::vector<MouseListener*> m_clickListeners;
	std::vector<MouseListener*> m_heldListeners;
public:
	virtual void Update() override;

	void AddClickListener(MouseListener* listener);
	void AddHeldListener(MouseListener* listener);
};