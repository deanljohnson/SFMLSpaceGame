#pragma once
#include <vector>
#include <memory>
#include "Component.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <array>
#include "EntityManager.h"

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
	template<typename T, typename... TArgs>
	T& AddComponent(TArgs&&... args);

	template<typename T>
	bool HasComponent() const;

	template<typename T>
	T& GetComponent() const;

	
	//****** Group Handling Methods ******
	bool HasGroup(Group group) const noexcept;

	void DeleteFromGroup(Group group) noexcept;

	void AddToGroup(Group group) noexcept;
};