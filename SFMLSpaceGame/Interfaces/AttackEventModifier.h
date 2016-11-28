#pragma once

class Event;

class AttackedEventModifier 
{
public:
	virtual ~AttackedEventModifier()
	{
	}

	virtual void Modify(Event& event) = 0;
};