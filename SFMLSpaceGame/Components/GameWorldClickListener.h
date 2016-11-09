#pragma once
#include <Components/Component.h>
#include <Interfaces/MouseListener.h>
#include <vector>
#include <DefaultSerializeable.h>

class GameWorldClickListener : public Component, public DefaultSerializeable<GameWorldClickListener>
{
private:
	enum ClickState { None, Down, Held, Click } m_clickedState;
	std::vector<MouseListener*> m_clickListeners;
	std::vector<MouseListener*> m_heldListeners;
public:
	explicit GameWorldClickListener(EntityID ent);

	virtual void Update() override;

	void AddClickListener(MouseListener* listener);
	void AddHeldListener(MouseListener* listener);
};