#pragma once
#include <Components/Component.h>
#include <functional>
#include <vector>

class GameWorldClickListener : public Component
{
private:
	enum ClickState { None, Down, Held, Click } m_clickedState;
	std::vector<std::function<void()>> m_clickListeners;
	std::vector<std::function<void()>> m_heldListeners;
public:
	virtual void Update() override;

	void AddClickListener(std::function<void()> listener);
	void AddHeldListener(std::function<void()> listener);
};