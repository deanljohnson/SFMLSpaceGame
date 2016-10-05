#pragma once
#include <string>
#include <UI/UIElement.h>
#include <ResourceLoader.h>
#include <memory>
#include <unordered_map>
#include <stack>
#include <UI/UI_Result.h>

#ifndef UI_ID
#define UI_ID unsigned long
#endif

#ifndef UI_ID_NULL
#define UI_ID_NULL unsigned long(0)
#endif


namespace
{
	inline void DoNothing(){}
	template<typename... T>
	inline void DoNothing(T... args){}
}


// Initializes a UIElement of the given type with the given args, assigning it's UI_ID to the given ID
// Subsequent calls will Display the element without passing/evaluating arguments
#define INIT_AND_DISPLAY(_Type_, _ID_, ...) ((_ID_ == 0) ? (_ID_ = UI::Init<_Type_>(__VA_ARGS__)) \
										    : (UI::Display(_ID_)))

// Initializes a UIElement of the given type with the given args, assigning it's UI_ID to the given ID
// Subsequent calls will Display the element and pass the given arguments to it's Refresh method
#define INIT_AND_REFRESH(_Type_, _ID_, ...) ((_ID_ == 0) ? (_ID_ = UI::Init<_Type_>(__VA_ARGS__)) \
										    : (UI::Display<_Type_>(_ID_, __VA_ARGS__)))

// Creates a transform hierarchy with the first argument as the parent.
// The transforms of all subsequent elements will be multiplied by the parents transform.
#define MAKE_HIERARCHY(_PARENT_, _CHILDONE_, ...) ([]()->UI_ID { UI::PushHierarchy(_PARENT_); \
																_CHILDONE_; DoNothing(__VA_ARGS__); \
																UI::PopHierarchy(); \
																return UI_ID_NULL; })()

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
			  lastUpdate{update},
			  parent{parentID}
		{
		}

		std::unique_ptr<UIElement> element;
		long lastUpdate;
		UI_Result result;
		UI_ID parent{ UI_ID_NULL };
	};

	static UI_ID m_nextID;
	static long m_updateCounter;

	static std::unordered_map<UI_ID, ElementRecord> m_elements;
	static std::vector<UI_ID> m_displayOrder;
	static std::stack<UI_ID> m_hierarchyIDs;

public:
	static void Init();

	static void Update();
	static void Render(sf::RenderTarget& target, sf::RenderStates& states);

	static UI_Result& GetResult(UI_ID id);

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

		// Create a record for the element
		m_elements.emplace(std::make_pair(id, ElementRecord(move(uPtr), m_updateCounter, m_hierarchyIDs.top())));

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
			it->second.parent = m_hierarchyIDs.top();
			m_displayOrder.push_back(prevID);

			// Call refresh
			// Note we don't need to cast to T
			it->second.element.get()->Refresh();

			m_displayOrder.push_back(prevID);

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
			it->second.parent = m_hierarchyIDs.top();
			m_displayOrder.push_back(prevID);

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

		// Create a record for the element
		m_elements.emplace(std::make_pair(id, ElementRecord(move(uPtr), m_updateCounter, m_hierarchyIDs.top())));

		m_displayOrder.push_back(id);

		// Return to the caller the ID for the newly created element
		return id;
	}
};