#pragma once
#include <cereal/access.hpp>
#include <EntityID.h>

template<class T>
class DefaultSerializeable
{
private:
	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		// Casting gives us access to entity variable
		ar(static_cast<T*>(this)->entity.GetID());
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<T>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);
	}
};