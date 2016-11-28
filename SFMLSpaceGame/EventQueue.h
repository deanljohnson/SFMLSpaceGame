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

	bool Get(EventType type, Event& target, bool markHandled = false);

	void Update();
	int Count() const;
	void Clear();
};