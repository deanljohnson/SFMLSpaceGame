// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <EventQueue.h>

EventQueue::EventQueue()
	: m_events(), 
	  m_pendingIndex(0)
{}

void EventQueue::Push(std::unique_ptr<Event> e)
{
	m_events.push_back(move(e));
}

bool EventQueue::Get(EventType type, Event* target, bool markHandled)
{
	// Iterate through events from the last update.
	// Anything after the pending index has been added 
	// to the event queue since the last call to 
	// EventQueue.Update
	for (int i = 0; i < m_pendingIndex; i++)
	{
		if (m_events[i]->type == type && !m_events[i]->handled)
		{
			if (markHandled)
				m_events[i]->handled = true;

			target = m_events[i].get();
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