#pragma once
#include <Components/Component.h>
#include <cereal/cereal.hpp>
#include <Item.h>

class OreVein : public Component
{
private:
	OreItem m_ore;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(GetEntityID(), 
			cereal::make_nvp("Ore", m_ore));
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<OreVein>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		ar(cereal::make_nvp("Ore", construct->m_ore));
	}
public:
	explicit OreVein(EntityID ent);

	// Attempts to mine the given amount from the ore vein. 
	// Returns the actual amount removed
	size_t Mine(size_t goalAmount);

	void SetAmount(size_t amt);
};