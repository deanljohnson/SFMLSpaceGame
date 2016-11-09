#pragma once
#include <vector>
#include <memory>
#include <array>
#include <functional>
#include <Components/Component.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Group.h>
#include <EntityID.h>
#include "EventQueue.h"

class EntityManager;

class Entity
{
private:
	bool m_alive{ true };
	bool m_active{ true };

	std::vector<std::unique_ptr<Component>> m_components;

	ComponentArray m_componentArray;
	ComponentBitset m_componentBitset;

	GroupBitset m_groupBitset;

	EntityID m_id;

public:
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

	//****** Component Handling Methods ******
	
	template<typename T>
	T& GetComponent() const
	{
		assert(HasComponent<T>());
		auto ptr{ m_componentArray[GetComponentTypeID<T>()] };
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
		//we only allow entities to have a single component of a type
		//assert(!HasComponent<T>());

		//Call the components constructor with the given args
		T* c(new T(m_id, std::forward<TArgs>(args)...));

		//wrap the raw pointer
		std::unique_ptr<Component> uPtr{ c };

		//add to our set of components
		m_components.emplace_back(move(uPtr));

		auto compID = GetComponentTypeID<T>();

		// The entity already has a component of this type
		// add it to the component linked list
		if (m_componentBitset[compID])
		{
			Component* existing = m_componentArray[compID];
			while (existing->next != nullptr) existing = existing->next;
			existing->next = c;
		}
		else
		{
			m_componentArray[compID] = c;
			m_componentBitset[compID] = true;
		}

		//return in case caller needs this
		return *c;
	}

	
	//****** Group Handling Methods ******
	bool HasGroup(Group group) const noexcept;

	void DeleteFromGroup(Group group) noexcept;

	void AddToGroup(Group group) noexcept;
};