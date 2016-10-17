#pragma once
#include <Event.h>
#include <EntityHandle.h>

class AttackedEventModifier 
{
public:
	virtual void Modify(Event& event) = 0;
};