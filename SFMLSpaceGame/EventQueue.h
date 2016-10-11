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

	bool Get(EventType type, Event& target)
	{
		auto it = m_events.begin();
		while (it != m_events.end())
		{
			if (it->type == type)
			{
				target = (*it);
				return true;
			}
			++it;
		}
		return false;
	}

	void Update();
	std::deque<Event>::iterator Begin();
	std::deque<Event>::iterator End();
	int Count();
	void Clear();
};