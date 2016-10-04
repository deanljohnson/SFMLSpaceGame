#pragma once
#include <string>
#include <map>
#include "UIElement.h"
#include <ResourceLoader.h>

#ifndef UI_ID
#define UI_ID unsigned long long
#endif
#include <memory>

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

class UIManager
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

	UI_ID m_nextID{ 0 };
	long m_updateCounter{ 0 };
	sf::RenderWindow* m_window;

	std::map<UI_ID, ElementRecord> m_elements;

public:
	explicit UIManager(sf::RenderWindow* window)
		: m_window(window)
	{}

	void Update();
	void Render(sf::RenderTarget& target, sf::RenderStates& states);

	UI_Result* GetResult(UI_ID id);

	template<typename T, typename... TArgs>
	UI_ID Display(UI_ID prevID, TArgs... args)
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