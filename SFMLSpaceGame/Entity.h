#pragma once
#include <vector>
#include <memory>
#include "Component.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <array>

class EntityManager;

using Group = size_t;

constexpr size_t maxGroups{ 32 };
using GroupBitset = std::bitset<maxGroups>;

class Entity
{
private:
	EntityManager& m_manager;

	bool m_alive{ true };
	std::vector<std::unique_ptr<Component>> m_components;

	ComponentArray m_componentArray;
	ComponentBitset m_componentBitset;

	GroupBitset m_groupBitset;
public:
	Entity(EntityManager& manager) 
		: m_manager(manager)
	{ }

	void Update();
	void Render(sf::RenderTarget& target);

	bool isAlive() const { return m_alive; }
	void Destroy() { m_alive = false; }


	//****** Component Handling Methods ******
	
	//For some reason the linker cant find this implementation if
	//I place it in the CPP file...
	template<typename T>
	T& GetComponent() const
	{
		//assert(HasComponent<T>());
		auto ptr{ m_componentArray[GetComponentTypeID<T>()] };
		return *static_cast<T*>(ptr);
	}

	template<typename T>
	bool HasComponent() const;

	template<typename T, typename... TArgs>
	T& AddComponent(TArgs&&... args);

	
	//****** Group Handling Methods ******
	bool HasGroup(Group group) const noexcept;

	void DeleteFromGroup(Group group) noexcept;

	void AddToGroup(Group group) noexcept;
};