#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <bitset>
#include <EntityHandle.h>

class Component;

using ComponentID = size_t;

namespace Internal
{
	inline ComponentID GetUniqueComponentID() noexcept
	{
		//static variable that will be incremented on each call
		static ComponentID lastID{ 0u };
		return lastID++;
	}
}

template<typename T>
inline ComponentID GetComponentTypeID() noexcept
{
	//make sure this is only called with type that inherit
	//from component
	static_assert(std::is_base_of<Component, T>::value,
		"T must inherit from Component");

	//static variable - only called when this function is called with a new type
	static ComponentID typeID{ Internal::GetUniqueComponentID() };
	return typeID;
}

constexpr size_t maxComponents{ 48 };
using ComponentBitset = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
private:
	bool m_active{ true };
public:
	Component();
	explicit Component(EntityID ent);

	Component* next{ nullptr }; // allows multiple components of a single type on an entity
	EntityHandle entity;

	virtual ~Component(){};

	virtual void Update(){};
	virtual void OnDestroy(){};
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states){};

	inline bool IsActive() const noexcept { return m_active; }
	inline void SetActive(bool active) noexcept
	{ 
		if (active == m_active) return; 
		m_active = active; 

		if (m_active) OnEnable();
		else OnDisable();
	}

	// Called when the component is enabled.
	// Not called when a component is created.
	virtual void OnEnable() {}

	// Called when a component is disabled.
	virtual void OnDisable() {}
};
