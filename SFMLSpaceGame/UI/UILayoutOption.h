#pragma once
#include <vector>
#include "UI_ID.h"

class UILayoutOption
{
public:
	UILayoutOption(const UILayoutOption& other)
		: type{other.type},
		  lastUpdate{other.lastUpdate}
	{
	}

	UILayoutOption(UILayoutOption&& other)
		: type{other.type},
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
	bool applied = false;

	void ApplyHorizontalGrouping();
	void ApplyVerticalGrouping();

public:
	enum LayoutType { NONE, HorizontalGroup, VerticalGroup } type;

	explicit UILayoutOption(LayoutType t)
		: type(t)
	{}

	long lastUpdate;

	void Init();
	void Apply();
	void Add(UI_ID id);
};