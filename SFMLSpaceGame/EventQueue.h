#pragma once
#include <deque>
#include <Event.h>

// A queue of events that saves events for a max of 2 updates
class EventQueue
{
private:
	std::deque<std::unique_ptr<Event>> m_events;
	int m_pendingIndex;

public:
	EventQueue();

	void Push(std::unique_ptr<Event> e);

	// Gets the next pending event of the given type.
	// The pointer returned from this method is only
	// guarenteed to be valid for the current update 
	// cycle
	template<EventType T>
	EventTypeType<T>* Get(bool markHandled = false)
	{
		// Iterate through events from the last update.
		// Anything after the pending index has been added 
		// to the event queue since the last call to 
		// EventQueue.Update
		for (int i = 0; i < m_pendingIndex; i++)
		{
			if (m_events[i]->type == T && !m_events[i]->handled)
			{
				if (markHandled)
					m_events[i]->handled = true;

				return static_cast<EventTypeType<T>*>(m_events[i].get());
			}
		}
		return nullptr;
	}

	// Gets the next pending event of the given type.
	// The pointer returned from this method is only
	// guarenteed to be valid for the current update 
	// cycle
	bool Get(EventType type, Event* target, bool markHandled = false);

	void Update();
	int Count() const;
	void Clear();
};