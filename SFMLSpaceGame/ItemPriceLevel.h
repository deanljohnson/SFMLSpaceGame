#pragma once
#include <ItemPrice.h>
#include <cereal/access.hpp>

struct ItemPriceLevel
{
private:
	friend class cereal::access;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(CEREAL_NVP(minAmount),
			CEREAL_NVP(maxPrice),
			CEREAL_NVP(targetAmount),
			CEREAL_NVP(targetPrice),
			CEREAL_NVP(maxAmount),
			CEREAL_NVP(minPrice));
	}
public:
	size_t minAmount{ 0 }; // At this amount, an agent will buy a given Item for maxPrice
	size_t targetAmount{ 0 }; // At this amount, an agent will buy a given Item for targetPrice
	size_t maxAmount{ 0 }; // At this amount, an agent will buy a given Item for minPrice
	
	Price minPrice{ 0 }; // At this price, an agent should have ~maxAmount of a given Item
	Price targetPrice{ 0 }; // At this price, an agent should have ~targetAmount of a given Item
	Price maxPrice{ 0 }; // At this price, an agent should have ~minAmount of a given Item
};