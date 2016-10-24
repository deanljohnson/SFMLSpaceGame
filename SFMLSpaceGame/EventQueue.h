#pragma once
#include <deque>
#include <Event.h>

// A queue of events that saves events for a max of 2 updates
class EventQueue
{
private:
	std::deque<Event> m_events;
	int m_pendingIndex;

public:
	EventQueue()
		: m_events(), 
		  m_pendingIndex(0)
	{}

	void Push(Event e);

	bool Get(EventType type, Event& target, bool markHandled = false)
	{
		// Iterate through events from the last update.
		// Anything after the pending index has been added 
		// to the event queue since the last call to 
		// EventQueue.Update
		for (int i = 0; i < m_pendingIndex; i++) 
		{
			if (m_events[i].type == type && !m_events[i].handled)
			{
				if (markHandled)
					m_events[i].handled = true;

				target = m_events[i];
				return true;
			}
		}
		return false;
	}

	void Update();
	int Count() const;
	void Clear();
};