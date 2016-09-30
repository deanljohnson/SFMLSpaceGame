#pragma once
#include <Box2d/Dynamics/b2Body.h>

class MouseListener 
{
public:
	virtual void OnClick(b2Vec2 pos) = 0;
	virtual void OnHeld(b2Vec2 pos) = 0;
};