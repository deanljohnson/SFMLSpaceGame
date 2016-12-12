#pragma once
#include <vector>
#include <DefaultSerializeable.h>

class MouseListener;

class GameWorldClickListener : public Component, public DefaultSerializeable<GameWorldClickListener>, public Updateable
{
private:
	enum ClickState { None, Down, Held, Click };

	ClickState m_leftClickedState;
	std::vector<MouseListener*> m_leftClickListeners;
	std::vector<MouseListener*> m_leftHeldListeners;

	ClickState m_rightClickedState;
	std::vector<MouseListener*> m_rightClickListeners;
	std::vector<MouseListener*> m_rightHeldListeners;

	ClickState m_middleClickedState;
	std::vector<MouseListener*> m_middleClickListeners;
	std::vector<MouseListener*> m_middleHeldListeners;

	void HandleLeftPress();
	void HandleLeftRelease();
	void HandleRightPress();
	void HandleRightRelease();
	void HandleMiddlePress();
	void HandleMiddleRelease();
public:
	explicit GameWorldClickListener(EntityID ent);

	virtual void Update() override;

	void AddClickListener(MouseListener* listener);
	void AddHeldListener(MouseListener* listener);
};