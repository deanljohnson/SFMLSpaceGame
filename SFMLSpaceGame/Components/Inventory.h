#pragma once
#include <Item.h>
#include <vector>
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

class Inventory : public Component, public Updateable
{
private:
	std::vector<std::shared_ptr<Item>> m_items;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_items);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Inventory>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		ar(construct->m_items);
	}
public:
	explicit Inventory(EntityID ent);

	virtual void Update() override;

	int GetCredits();
	void SetCredits(int c);

	void AddItem(std::shared_ptr<Item> item);
	void RemoveItem(std::shared_ptr<Item> item);

	typedef std::vector<std::shared_ptr<Item>>::iterator iterator;
	typedef std::vector<std::shared_ptr<Item>>::const_iterator const_iterator;

	iterator begin() { return m_items.begin(); }
	iterator end() { return m_items.end(); }
};