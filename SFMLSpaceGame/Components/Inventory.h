#pragma once
#include <Components/Component.h>
#include <Item.h>
#include <vector>

class Inventory : public Component
{
private:
	std::vector<Item> m_items;
public:
	Inventory();

	virtual void Update() override;

	int GetCredits();
	void SetCredits(int c);

	void AddItem(const Item& item);
	void RemoveItem(const Item& item);

	typedef std::vector<Item>::iterator iterator;
	typedef std::vector<Item>::const_iterator const_iterator;

	iterator begin() { return m_items.begin(); }
	iterator end() { return m_items.end(); }
};