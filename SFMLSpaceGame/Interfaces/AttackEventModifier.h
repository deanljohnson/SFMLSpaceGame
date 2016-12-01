#pragma once

class AttackedEvent;

class AttackedEventModifier 
{
public:
	virtual ~AttackedEventModifier()
	{
	}

	virtual void Modify(AttackedEvent* event) = 0;
};