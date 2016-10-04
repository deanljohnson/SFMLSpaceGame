#pragma once
#include <string>
#include <map>
#include "UIElement.h"
#include <ResourceLoader.h>
#include <memory>

#ifndef UI_ID
#define UI_ID unsigned long
#endif

// Initializes a UIElement of the given type with the given args, assigning it's UI_ID to the given ID
// Subsequent calls will Display the element without passing/evaluating arguments
#define INIT_AND_DISPLAY(_Type_, _ID_, ...) if (_ID_ == 0) _ID_ = UI::Display<_Type_>(_ID_, __VA_ARGS__); \
										    else UI::Display<_Type_>(_ID_)

namespace sf{
	class RenderWindow;
}

union UI_Result
{
	UI_Result(const UI_Result& other)
		: booleanValue{other.booleanValue},
		  intValue{other.intValue},
		  stringValue{other.stringValue}
	{
	}

	UI_Result(UI_Result&& other)
		: booleanValue{other.booleanValue},
		  intValue{other.intValue},
		  stringValue{std::move(other.stringValue)}
	{
	}

	UI_Result& operator=(UI_Result other)
	{
		using std::swap;
		swap(*this, other);
		return *this;
	}

	UI_Result() { booleanValue = false; }
	~UI_Result() {}
	bool booleanValue;
	int intValue;
	std::string stringValue;
};

class UI
{
private:
	struct ElementRecord
	{
		explicit ElementRecord(std::unique_ptr<UIElement> elem, long update)
			: element(move(elem)),
			  lastUpdate{update}
		{
		}

		std::unique_ptr<UIElement> element;
		long lastUpdate;
		UI_Result result;
	};

	static UI_ID m_nextID;
	static long m_updateCounter;

	static std::map<UI_ID, ElementRecord> m_elements;

public:
	static void Update();
	static void Render(sf::RenderTarget& target, sf::RenderStates& states);

	static UI_Result* GetResult(UI_ID id);

	template<typename T>
	static UI_ID Display(UI_ID prevID)
	{
		static_assert(std::is_base_of<UIElement, T>::value,
			"T must inherit from UIElement");

		auto it = m_elements.find(prevID);
		if (it != m_elements.end())
		{
			// mark the element as having just been updated
			it->second.lastUpdate = m_updateCounter;

			// Call refresh
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
		m_elements.emplace(std::make_pair(id, ElementRecord(move(uPtr), m_updateCounter)));

		// Return to the caller the ID for the newly created element
		return id;
	}
};