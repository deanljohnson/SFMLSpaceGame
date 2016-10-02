#pragma once
#include <vector>
#include <memory>
#include <Components/Component.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <array>
#include <Box2D/Dynamics/b2World.h>
#include <bitset>

class EntityManager;

using Group = size_t;

constexpr size_t maxGroups{ 32 };
using GroupBitset = std::bitset<maxGroups>;

class Entity
{
public:
	Entity(const Entity& other) = delete;

	Entity(Entity&& other)
		: m_alive{other.m_alive},
		  m_components{std::move(other.m_components)},
		  m_componentBitset{std::move(other.m_componentBitset)},
		  m_groupBitset{std::move(other.m_groupBitset)},
		  m_manager{other.m_manager},
		  m_world{other.m_world}
	{
		m_componentArray = other.m_componentArray;
	}

	Entity& operator=(Entity other)
	{
		using std::swap;
		swap(*this, other);
		return *this;
	}

private:
	bool m_alive{ true };
	std::vector<std::unique_ptr<Component>> m_components;
	
	ComponentArray m_componentArray;
	ComponentBitset m_componentBitset;

	GroupBitset m_groupBitset;

	EntityManager* m_manager;
	b2World* m_world;

public:
	Entity(EntityManager* manager, b2World* world) 
		: m_manager(manager), m_world(world)
	{ }

	void Update();
	void Render(sf::RenderTarget& target, sf::RenderStates& states);

	bool isAlive() const { return m_alive; }
	void Destroy() { m_alive = false; }

	b2World* GetWorld() const { return m_world; }
	EntityManager* GetManager() const { return m_manager; }

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
		T* c(new T(std::forward<TArgs>(args)...));

		//Set the owner to this
		c->entity = this;

		//wrap the raw pointer
		std::unique_ptr<Component> uPtr{ c };

		//add to our set of components
		m_components.emplace_back(move(uPtr));

		m_componentArray[GetComponentTypeID<T>()] = c;
		m_componentBitset[GetComponentTypeID<T>()] = true;

		c->Init();

		//return in case caller needs this
		return *c;
	}

	
	//****** Group Handling Methods ******
	bool HasGroup(Group group) const noexcept;

	void DeleteFromGroup(Group group) noexcept;

	void AddToGroup(Group group) noexcept;
};