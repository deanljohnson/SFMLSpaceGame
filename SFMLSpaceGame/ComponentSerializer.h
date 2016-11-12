#pragma once
#include <cereal\cereal.hpp>
#include <cereal\archives\json.hpp>
#include <cereal\types\memory.hpp>
#include <Serializer.h>
#include <Components\Component.h>

class Entity;

class ComponentSerializer
{
private:
	template<class T>
	static bool LoadComponents(ComponentID id, SERIALIZATION_IN_ARCHIVE& ar, Entity& ent) 
	{
		if (id != GetComponentTypeID<T>())
			return false;

		std::unique_ptr<T> ptr;
		ar(ptr);
		ent.SetComponent<T>(ptr.release());

		return true;
	}

	template<class TCompA, class TCompB, class... TCompTypes>
	static void LoadComponents(ComponentID id, SERIALIZATION_IN_ARCHIVE& ar, Entity& ent)
	{
		if (!LoadComponents<TCompA>(id, ar, ent))
			LoadComponents<TCompB, TCompTypes...>(id, ar, ent);
	}

	template<class T>
	static bool SaveComponents(ComponentID id, SERIALIZATION_OUT_ARCHIVE& ar, const Entity& ent)
	{
		if (id != GetComponentTypeID<T>())
			return false;

		std::unique_ptr<T> comp{ &ent.GetComponent<T>() };
		ar(id);
		ar(comp);

		while (comp->next != nullptr) 
		{
			comp = std::unique_ptr<T>(static_cast<T*>(comp.release()->next));
			ar(id);
			ar(comp);
		}
		comp.release();
		return true;
	}

	template<class TCompA, class TCompB, class... TCompTypes>
	static void SaveComponents(ComponentID id, SERIALIZATION_OUT_ARCHIVE& ar, const Entity& ent)
	{
		if (!SaveComponents<TCompA>(id, ar, ent))
			SaveComponents<TCompB, TCompTypes...>(id, ar, ent);
	}

	static void Load(ComponentID id, SERIALIZATION_IN_ARCHIVE& ar, Entity& ent);
	static void Save(ComponentID id, SERIALIZATION_OUT_ARCHIVE& ar, const Entity& ent);
public:
	static void Serialize(SERIALIZATION_IN_ARCHIVE& ar, Entity& ent);
	static void Serialize(SERIALIZATION_OUT_ARCHIVE& ar, const Entity& ent);
};