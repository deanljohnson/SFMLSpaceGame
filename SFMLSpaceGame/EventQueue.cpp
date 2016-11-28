#include "stdafx.h"
#include <EventQueue.h>

void EventQueue::Push(Event e)
{
	m_events.push_back(e);
}

bool EventQueue::Get(EventType type, Event& target, bool markHandled)
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

void EventQueue::Update()
{
	// Erase events from the last update
	m_events.erase(m_events.begin(), m_events.begin() + m_pendingIndex);

	// Set our new pending index to the end of the list
	m_pendingIndex = m_events.size();
}

int EventQueue::Count() const { return m_events.size(); }

void EventQueue::Clear() 
{
	m_events.clear();
	m_pendingIndex = 0;
}