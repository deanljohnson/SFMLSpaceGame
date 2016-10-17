#include <EventQueue.h>

void EventQueue::Push(Event e)
{
	m_events.push_back(e);
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