#pragma once
#include <deque>
#include <Event.h>

// A queue of events that saves events for a max of 2 updates
class EventQueue
{
private:
	std::deque<Event> m_events;
	int m_lastEventsSinceUpdate{ 0 };
	int m_eventsSinceUpdate{ 0 };

public:
	void Push(Event e);
	void Update();
	std::deque<Event>::iterator Begin();
	std::deque<Event>::iterator End();
	int Count();
};