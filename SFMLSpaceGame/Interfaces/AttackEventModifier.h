#pragma once
#include <Event.h>

class AttackedEventModifier 
{
public:
	virtual void Modify(Event& event) = 0;
};