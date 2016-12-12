#pragma once
#include <cereal\cereal.hpp>
#include <cereal\archives\json.hpp>
#include <cereal\archives\binary.hpp>
#include <cereal\types\memory.hpp>
#include <Components\Component.h>

class Entity;

class ComponentSerializer
{
private:
	template<class Archive, class T>
	static bool LoadComponents(ComponentID id, Archive& ar, Entity& ent)
	{
		if (id != GetComponentTypeID<T>())
			return false;

		std::unique_ptr<T> ptr;
		ar(ptr);
		ent.SetComponent<T>(ptr.release());

		return true;
	}

	template<class Archive, class TCompA, class TCompB, class... TCompTypes>
	static void LoadComponents(ComponentID id, Archive& ar, Entity& ent)
	{
		if (!LoadComponents<Archive, TCompA>(id, ar, ent))
			LoadComponents<Archive, TCompB, TCompTypes...>(id, ar, ent);
	}

	template<class Archive, class T>
	static bool SaveComponents(ComponentID id, Archive& ar, const Entity& ent)
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

	template<class Archive, class TCompA, class TCompB, class... TCompTypes>
	static void SaveComponents(ComponentID id, Archive& ar, const Entity& ent)
	{
		if (!SaveComponents<Archive, TCompA>(id, ar, ent))
			SaveComponents<Archive, TCompB, TCompTypes...>(id, ar, ent);
	}

	// we define these individually so that we don't have to pull the Components
	// header into this header
	static void Load(ComponentID id, cereal::JSONInputArchive& ar, Entity& ent);
	static void Load(ComponentID id, cereal::BinaryInputArchive& ar, Entity& ent);
	static void Save(ComponentID id, cereal::JSONOutputArchive& ar, const Entity& ent);
	static void Save(ComponentID id, cereal::BinaryOutputArchive& ar, const Entity& ent);
public:
	template<class Archive>
	static void SerializeIn(Archive& ar, Entity& ent) 
	{
		ComponentID id;
		ar(id);

		while (id != std::numeric_limits<ComponentID>::max())
		{
			Load(id, ar, ent);

			ar(id); // get the next component ID
		}
	}

	template<class Archive>
	static void SerializeOut(Archive& ar, const Entity& ent) 
	{
		for (int i = 0; i < maxComponents; i++)
		{
			if (ent.m_componentBitset[i])
			{
				Save(i, ar, ent);
			}
		}

		// ComponentID::max is used as a flag to indicate the end of 
		// component section in the serialized representation
		ar(std::numeric_limits<ComponentID>::max());
	}
};