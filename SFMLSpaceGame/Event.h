#pragma once

struct AttackedEvent
{
	
};

enum EventType
{
	Attacked
};

struct Event
{
	EventType type;
	union
	{
		AttackedEvent attacked;
	};
};