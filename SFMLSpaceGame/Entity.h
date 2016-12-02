#pragma once
#include <vector>
#include <memory>
#include <array>
#include <functional>
#include <Components/Component.h>
#include <ComponentSerializer.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Group.h>
#include <EntityID.h>
#include "EventQueue.h"
#include <cereal\access.hpp>
#include <cereal\types\bitset.hpp>
#include <EntityInitializer.h>

class EntityManager;

class Entity
{
private:
	bool m_alive{ true };
	bool m_active{ true };

	std::multimap<ComponentID, std::unique_ptr<Component>> m_components;

	ComponentArray m_componentArray;
	ComponentBitset m_componentBitset;

	GroupBitset m_groupBitset;
	std::bitset<EntityInitializer::Type::Count> m_initializerBitset;

	EntityID m_id;

	template<class T>
	void SetComponent(T* comp) 
	{
		//wrap the raw pointer
		std::unique_ptr<Component> uPtr{ comp };

		auto compTypeID = GetComponentTypeID<T>();

		//add to our set of components
		m_components.emplace(compTypeID, move(uPtr));

		// The entity already has a component of this type
		// add it to the component linked list
		if (m_componentBitset[compTypeID])
		{
			Component* existing = m_componentArray[compTypeID];
			while (existing->next != nullptr) 
			{
				existing = existing->next;
			} 
			existing->next = comp;
		}
		// New component entirely
		else
		{
			m_componentArray[compTypeID] = comp;
			m_componentBitset[compTypeID] = true;
		}
	}

	// ComponentSerializer needs to access component arrays
	// but we don't want anything else to be able to
	friend class ComponentSerializer;

	// Give cereal access to internals
	friend class cereal::access;

	void OnDeserialize();
	void ApplyInitializers();
	
	template<class Archive>
	void save(Archive& ar) const
	{
		ar(m_id, m_alive, m_active, m_groupBitset, m_initializerBitset);
		// Save components
		ComponentSerializer::SerializeOut<Archive>(ar, *this);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(m_id, m_alive, m_active, m_groupBitset, m_initializerBitset);
		OnDeserialize();

		// Load components
		ComponentSerializer::SerializeIn<Archive>(ar, *this);
		ApplyInitializers();
	}
public:
	typedef int ComponentID;

	// used only for serialization
	Entity() : m_id(ENTITY_ID_NULL) {}

	Entity(const Entity& other) = delete;

	Entity(Entity&& other)
		: m_alive{other.m_alive},
		  m_active{other.m_active},
		  m_components{std::move(other.m_components)},
		  m_componentArray{},
		  m_componentBitset{std::move(other.m_componentBitset)},
		  m_groupBitset{std::move(other.m_groupBitset)},
		  m_id{other.m_id},
		  destroyCallback{std::move(other.destroyCallback)},
		  events{}
	{
	}

	Entity& operator=(Entity& other)
	{
		using std::swap;
		swap(*this, other);
		return *this;
	}

	// Declared only for serialization support
	//Entity() {}
	explicit Entity(EntityID id) 
		: m_id(id)
	{
	}

	std::function<void(Entity*)> destroyCallback{ nullptr };
	EventQueue events;

	void Update();
	void Render(sf::RenderTarget& target, sf::RenderStates& states);

	inline bool isAlive() const { return m_alive; }
	inline void Destroy() { m_alive = false; OnDestroy(); }
	void OnDestroy();

	inline bool isActive() const { return m_active; }
	inline void SetActive(bool val) { m_active = val; }

	inline EntityID GetID() const { return m_id; }

	void ApplyInitializer(EntityInitializer::Type init);

	//****** Component Handling Methods ******
	
	template<typename T>
	T& GetComponent(int index = 0) const
	{
		assert(HasComponent<T>());
		auto ptr{ m_componentArray[GetComponentTypeID<T>()] };

		while (index > 0) 
		{
			index--;
			ptr = ptr->next;
		}

		return *static_cast<T*>(ptr);
	}

	template<typename T>
	bool HasComponent() const 
	{
		return m_componentBitset[GetComponentTypeID<T>()];
	}

	template<typename T, typename... TArgs>
	T& AddComponent(TArgs... args) 
	{
		//Call the components constructor with the given args
		T* c(new T(m_id, std::forward<TArgs>(args)...));

		SetComponent<T>(c);

		//return in case caller needs this
		return *c;
	}

	// returns an identifier that can be used
	// in GetComponent calls to get a specific
	// component when there are multiple 
	// components of the same type on the entity
	template<typename T>
	ComponentID GetComponentID(T& comp) const
	{
		ComponentID id = 0;
		Component* current = m_componentArray[GetComponentTypeID<T>()];
		while (current != &comp)
		{
			current = current->next;
			id++;
		}

		return id;
	}
	
	//****** Group Handling Methods ******
	bool HasGroup(Group group) const noexcept;

	void DeleteFromGroup(Group group) noexcept;

	void AddToGroup(Group group) noexcept;

	static std::string GetTypeName() { static const std::string name = "entity"; return name; }
};