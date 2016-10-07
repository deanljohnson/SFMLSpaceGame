#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <bitset>
#include <array>

class Entity;
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

constexpr size_t maxComponents{ 32 };
using ComponentBitset = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Component() : entity(nullptr) {}

	Entity* entity{ nullptr };
	bool active{ true };

	virtual ~Component(){};

	virtual void Init(){}
	virtual void Update(){};
	virtual void Render(sf::RenderTarget& target, sf::RenderStates& states){};
};
