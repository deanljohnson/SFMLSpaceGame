#pragma once
#include <assert.h>

class Item
{
public:

	enum class ItemType
	{
		Credits
	};

	struct Credits
	{
		unsigned int amount;

		void Stack(Credits other) { amount += other.amount; }
	};

	union
	{
		Credits credits;
	};

	ItemType type;

	void Stack(Item other)
	{
		assert(type == other.type);

		switch (other.type)
		{
		case ItemType::Credits:
			credits.Stack(other.credits);
		}
	}
};