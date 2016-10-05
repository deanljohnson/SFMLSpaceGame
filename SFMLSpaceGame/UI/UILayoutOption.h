#pragma once
#include <vector>
#include "UI_ID.h"
#include <SFML/System/Vector2.hpp>

class UILayoutOption
{
public:
	UILayoutOption(const UILayoutOption& other)
		: position{other.position},
		  type{other.type},
		  lastUpdate{other.lastUpdate}
	{
	}

	UILayoutOption(UILayoutOption&& other)
		: position{std::move(other.position)},
		  type{other.type},
		  lastUpdate{other.lastUpdate}
	{
	}

	UILayoutOption& operator=(UILayoutOption other)
	{
		using std::swap;
		swap(*this, other);
		return *this;
	}

private:
	std::vector<UI_ID> m_elementIDs;
	sf::Vector2f position;
	bool applied = false;

	void ApplyHorizontalGrouping();
	void ApplyVerticalGrouping();
	void ApplyCenterHorizontal();

public:
	enum LayoutType { NONE, HorizontalGroup, VerticalGroup, CenterHorizontal } type;

	explicit UILayoutOption(LayoutType t, sf::Vector2f pos = sf::Vector2f(0,0))
		: position(pos), type(t)
	{}

	long lastUpdate;

	void Init();
	void Apply();
	void Add(UI_ID id);
};