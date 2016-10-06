#pragma once
#include <UI/UILayoutOption.h>
#include <UI/UIElement.h>
#include <UI/UIHorizontalGroup.h>
#include <UI/UIVerticalGroup.h>
#include <ResourceLoader.h>
#include <memory>
#include <unordered_map>
#include <stack>
#include "UI_ID.h"
#include <UI/UI_Result.h>

#ifndef INIT_AND_DISPLAY
// Initializes a UIElement of the given type with the given args, assigning it's UI_ID to the given ID
// Subsequent calls will Display the element without passing/evaluating arguments
#define INIT_AND_DISPLAY(_Type_, _ID_, ...) ((_ID_ == UI_ID_NULL) \
											? (_ID_ = UI::Init<_Type_>(__VA_ARGS__)) \
										    : (UI::Display(_ID_)))
#endif

#ifndef INIT_AND_REFRESH
// Initializes a UIElement of the given type with the given args, assigning it's UI_ID to the given ID
// Subsequent calls will Display the element and pass the given arguments to it's Refresh method
#define INIT_AND_REFRESH(_Type_, _ID_, ...) ((_ID_ == UI_ID_NULL) \
											? (_ID_ = UI::Init<_Type_>(__VA_ARGS__)) \
										    : (UI::Display<_Type_>(_ID_, __VA_ARGS__)))
#endif

#ifndef MAKE_HIERARCHY
// Creates a transform hierarchy with the first argument as the parent.
// The transforms of all subsequent elements will be multiplied by the parents transform.
#define MAKE_HIERARCHY(_PARENT_, ...)  (UI::PushHierarchy(_PARENT_), \
									   __VA_ARGS__, \
									   UI::PopHierarchy())
#endif

// Automatically lays the given elements out horizontally. Within a horizontal group,
// an elements transform is relative to the previous elements right side and a y value of 0
#ifndef HORIZONTAL_GROUP
#define HORIZONTAL_GROUP(_LAYOUT_ID_, ...) (MAKE_HIERARCHY(INIT_AND_DISPLAY(UIHorizontalGroup, _LAYOUT_ID_), __VA_ARGS__), _LAYOUT_ID_)
#endif


// Automatically lays the given elements out vertically. Within a vertical group,
// an elements transform is relative to the previous elements bottom and a x value of 0
#ifndef VERTICAL_GROUP
#define VERTICAL_GROUP(_LAYOUT_ID_, ...) (MAKE_HIERARCHY(INIT_AND_DISPLAY(UIVerticalGroup, _LAYOUT_ID_), __VA_ARGS__), _LAYOUT_ID_)
#endif

namespace sf{
	class Event;
	class RenderWindow;
}

class UI
{
private:
	struct ElementRecord
	{
		ElementRecord(){}
		explicit ElementRecord(std::unique_ptr<UIElement> elem, long update, UI_ID parentID = UI_ID_NULL)
			: element(move(elem)),
			  lastUpdate{update}
		{
		}

		std::unique_ptr<UIElement> element;
		long lastUpdate;
		UI_Result result;
		short childCount{ 0 };
	};

	static UI_ID m_nextID;
	static long m_updateCounter;

	static std::unordered_map<UI_ID, ElementRecord> m_elements;
	static std::vector<UI_ID> m_displayOrder;
	static std::stack<UI_ID> m_hierarchyIDs;
	static std::vector<UI_ID> m_rootIDs;

public:
	static void Init();

	static void Update();
	static void Render(sf::RenderTarget& target, sf::RenderStates& states);

	static UI_Result* GetResult(UI_ID id);
	static UIElement* Get(UI_ID id)
	{
		auto it = m_elements.find(id);
		if (it == m_elements.end()) return nullptr;
		return it->second.element.get();
	}

	static bool HandleEvent(const sf::Event& event);

	static void PushHierarchy(UI_ID parentID);
	static void PopHierarchy();

	template<typename T, typename... TArgs>
	static UI_ID Init(TArgs... args)
	{
		static_assert(std::is_base_of<UIElement, T>::value,
			"T must inherit from UIElement");

		// Create the new UIElement
		T* elem(new T(std::forward<TArgs>(args)...));

		// Wrap the raw pointer
		std::unique_ptr<UIElement> uPtr{ elem };

		// Get an ID for this new element
		UI_ID id = m_nextID++;
		uPtr->ID = id;

		// Create a record for the element
		m_elements.emplace(std::make_pair(id, ElementRecord(move(uPtr), m_updateCounter, m_hierarchyIDs.top())));

		// Increase it's parents child count
		if (m_hierarchyIDs.size() > 1)
		{
			m_elements.find(UI_ID(m_hierarchyIDs.top()))->second.childCount++;
			m_elements.find(UI_ID(m_hierarchyIDs.top()))->second.element->children.push_back(elem);
		} else {
			m_rootIDs.push_back(id);
		}

		m_displayOrder.push_back(id);

		// Return to the caller the ID for the newly created element
		return id;
	}

	static UI_ID Display(UI_ID prevID)
	{
		auto it = m_elements.find(prevID);
		if (it != m_elements.end())
		{
			// mark the element as having just been updated
			it->second.lastUpdate = m_updateCounter;

			// keep track of where this object is in the display hierarchy
			m_displayOrder.push_back(prevID);

			// Increase it's parents child count
			if (m_hierarchyIDs.size() > 1)
			{
				m_elements.find(UI_ID(m_hierarchyIDs.top()))->second.childCount++;
				m_elements.find(UI_ID(m_hierarchyIDs.top()))->second.element->children.push_back(it->second.element.get());
			} else {
				m_rootIDs.push_back(prevID);
			}
			// Call refresh
			// Note we don't need to cast to T
			it->second.element.get()->Refresh();

			// This was a pre-existing item so it keeps it's ID
			return prevID;
		}

		throw prevID + " is not a pre-existing UI_ID";
	}

	template<typename T, typename... TArgs>
	static UI_ID Display(UI_ID prevID, TArgs... args)
	{
		static_assert(std::is_base_of<UIElement, T>::value,
			"T must inherit from UIElement");

		auto it = m_elements.find(prevID);
		if (it != m_elements.end())
		{
			// mark the element as having just been updated
			it->second.lastUpdate = m_updateCounter;

			// keep track of where this object is in the display hierarchy
			m_displayOrder.push_back(prevID);

			// Increase it's parents child count
			if (m_hierarchyIDs.size() > 1)
			{
				m_elements.find(UI_ID(m_hierarchyIDs.top()))->second.childCount++;
				m_elements.find(UI_ID(m_hierarchyIDs.top()))->second.element->children.push_back(it->second.element.get());
			}
			else {
				m_rootIDs.push_back(prevID);
			}

			// Call refresh of type T with the given args
			static_cast<T*>(it->second.element.get())->Refresh(std::forward<TArgs>(args)...);

			// This was a pre-existing item so it keeps it's ID
			return prevID;
		}

		// Create the new UIElement
		T* elem(new T(std::forward<TArgs>(args)...));

		// Wrap the raw pointer
		std::unique_ptr<UIElement> uPtr{ elem };

		// Get an ID for this new element
		UI_ID id = m_nextID++;
		uPtr->ID = id;

		// Create a record for the element
		m_elements.emplace(std::make_pair(id, ElementRecord(move(uPtr), m_updateCounter, m_hierarchyIDs.top())));

		// Increase it's parents child count
		if (m_hierarchyIDs.size() > 1)
		{
			m_elements.find(UI_ID(m_hierarchyIDs.top()))->second.childCount++;
			m_elements.find(UI_ID(m_hierarchyIDs.top()))->second.element->children.push_back(elem);
		}
		else {
			m_rootIDs.push_back(id);
		}

		m_displayOrder.push_back(id);

		// Return to the caller the ID for the newly created element
		return id;
	}
};