#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <bitset>
#include <array>

class Entity;
class Component;
class Sensor;

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
	Component() : next(nullptr), entity(nullptr) {}

	Component* next{ nullptr }; // allows multiple components of a single type on an entity
	Entity* entity{ nullptr };

	virtual ~Component(){};

	virtual void Init(){}
	virtual void Update(){};
	virtual void OnDestroy(){};
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states){};

	inline bool IsActive() const noexcept { return m_active; }
	inline void SetActive(bool active) noexcept { m_active = active; }

	// Makes this component's activation state reliant on
	// the given Sensor's triggered state.
	void AttachToSensor(Sensor* sensor);
};
