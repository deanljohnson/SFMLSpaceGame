#pragma once

struct b2Vec2;

class MouseListener 
{
public:
	virtual ~MouseListener()
	{
	}

	virtual void OnClick(b2Vec2 pos) = 0;
	virtual void OnHeld(b2Vec2 pos) = 0;
};