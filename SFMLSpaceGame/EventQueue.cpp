#include <EventQueue.h>

void EventQueue::Push(Event e)
{
	m_events.push_back(e);
}

void EventQueue::Update()
{
	m_eventsSinceUpdate = m_events.size() - m_lastEventsSinceUpdate;

	// Remove events that are too old
	while (m_lastEventsSinceUpdate > 0)
	{
		m_events.pop_front();
		m_lastEventsSinceUpdate--;
	}
	m_lastEventsSinceUpdate = m_eventsSinceUpdate;
	m_eventsSinceUpdate = 0;
}


std::deque<Event>::iterator EventQueue::Begin()
{
	return m_events.begin();
}

std::deque<Event>::iterator EventQueue::End()
{
	return m_events.end();
}

int EventQueue::Count() const { return m_events.size(); }

void EventQueue::Clear() 
{
	m_events.clear();
	m_lastEventsSinceUpdate = 0;
	m_eventsSinceUpdate = 0;
}