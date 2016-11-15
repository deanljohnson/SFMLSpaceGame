#pragma once

struct b2Vec2;

class MouseListener 
{
public:
	sf::Mouse::Button button{sf::Mouse::Button::Left};

	virtual ~MouseListener()
	{
	}

	virtual void OnClick(b2Vec2 pos) = 0;
	virtual void OnHeld(b2Vec2 pos) = 0;
};