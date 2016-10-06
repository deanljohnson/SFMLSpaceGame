#pragma once
#include <vector>
#include "UI_ID.h"
#include <SFML/System/Vector2.hpp>
#include <UI/UIElement.h>

class UILayoutOption
{
public:
	UILayoutOption(const UILayoutOption& other)
		: position{other.position},
		  m_layoutOperations{ other.m_layoutOperations },
		  lastUpdate{other.lastUpdate}
	{
	}

	UILayoutOption(UILayoutOption&& other)
		: position{std::move(other.position)},
		  m_layoutOperations{std::move(other.m_layoutOperations)},
		  lastUpdate{other.lastUpdate}
	{
	}

	UILayoutOption& operator=(UILayoutOption other)
	{
		using std::swap;
		swap(*this, other);
		return *this;
	}
	enum LayoutType { NONE, HorizontalGroup, VerticalGroup, CenterHorizontal };
private:
	std::vector<UI_ID> m_elementIDs;
	std::vector<LayoutType> m_layoutOperations;
	sf::Vector2f position;
	bool applied = false;

	void ApplyHorizontalGrouping();
	void ApplyVerticalGrouping();
	void ApplyCenterHorizontal();

public:
	UILayoutOption() {}
	explicit UILayoutOption(LayoutType t, sf::Vector2f pos = sf::Vector2f(0,0))
		: position(pos)
	{
		m_layoutOperations.push_back(t);
	}
	explicit UILayoutOption(std::initializer_list<LayoutType> layoutTypes, sf::Vector2f pos = sf::Vector2f(0, 0))
		: m_layoutOperations(layoutTypes),
		  position(pos)
	{}

	long lastUpdate;

	void Init();
	void Apply();
	void Add(UI_ID id);
	
	void InitLayout();
	sf::Transform ApplyLayout(UIElement* elem);
};