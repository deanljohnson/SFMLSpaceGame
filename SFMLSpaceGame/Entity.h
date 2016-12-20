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
#include <EventQueue.h>
#include <cereal\access.hpp>
#include <cereal\types\bitset.hpp> // needed to serialize component bitset
#include <EntityInitializer.h>
#include <Components/Interfaces/Updateable.h>
#include <Components/Interfaces/Renderable.h>

class EntityManager;

class Entity
{
private:
	bool m_alive{ true };
	bool m_active{ true };
	std::string m_name{""};

	std::multimap<ComponentID, std::unique_ptr<Component>> m_components;
	std::vector<ComponentID> m_updateableComponents;
	std::vector<ComponentID> m_renderableComponents;

	ComponentArray m_componentArray;
	ComponentBitset m_componentBitset;

	GroupBitset m_groupBitset;
	std::bitset<EntityInitializer::Type::Count> m_initializerBitset;

	EntityID m_id;

	template<class T,
		typename std::enable_if<!std::is_base_of<Updateable, T>::value
		&& !std::is_base_of<Renderable, T>::value>::type* = nullptr>
	void EmplaceComponent(ComponentID id, T* comp)
	{
		//add to our set of components
		m_components.emplace(id, std::unique_ptr<T>(comp));
	}

	template<class T,
		typename std::enable_if<std::is_base_of<Updateable, T>::value 
		&& std::is_base_of<Renderable, T>::value>::type* = nullptr>
	void EmplaceComponent(ComponentID id, T* comp)
	{
		//add to our set of components
		m_components.emplace(id, std::unique_ptr<T>(comp));

		m_updateableComponents.insert(
			upper_bound(m_updateableComponents.begin(), m_updateableComponents.end(), id), 
			id);
		m_renderableComponents.insert(
			upper_bound(m_renderableComponents.begin(), m_renderableComponents.end(), id),
			id);
	}

	template<class T,
		typename std::enable_if<std::is_base_of<Updateable, T>::value
		&& !std::is_base_of<Renderable, T>::value>::type* = nullptr>
		void EmplaceComponent(ComponentID id, T* comp)
	{
		//add to our set of components
		m_components.emplace(id, std::unique_ptr<T>(comp));

		m_updateableComponents.insert(
			upper_bound(m_updateableComponents.begin(), m_updateableComponents.end(), id),
			id);
	}

	template<class T,
		typename std::enable_if<!std::is_base_of<Updateable, T>::value
		&& std::is_base_of<Renderable, T>::value>::type* = nullptr>
		void EmplaceComponent(ComponentID id, T* comp)
	{
		//add to our set of components
		m_components.emplace(id, std::unique_ptr<T>(comp));

		m_renderableComponents.insert(
			upper_bound(m_renderableComponents.begin(), m_renderableComponents.end(), id),
			id);
	}

	template<class T>
	void SetComponent(T* comp) 
	{
		auto compTypeID = GetComponentTypeID<T>();

		EmplaceComponent<T>(compTypeID, comp);

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
		ar(cereal::make_nvp("ID", m_id));

		// Serialize the entity's name, only if it has one
		if (!m_name.empty())
			ar(cereal::make_nvp("name", m_name));

		ar(cereal::make_nvp("active", m_active),
			cereal::make_nvp("groups", m_groupBitset),
			cereal::make_nvp("initializers", m_initializerBitset));

		// Save components
		ComponentSerializer::SerializeOut<Archive>(ar, *this);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		ar(cereal::make_nvp("ID", m_id));

		// Load the entities name, if the data has a name field
		std::string nextNode = ar.getNodeName();
		if (nextNode == "name")
			ar(m_name, cereal::make_nvp("active", m_active));
		else
			ar(m_active);

		ar(cereal::make_nvp("groups", m_groupBitset),
			cereal::make_nvp("initializers", m_initializerBitset));

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

	explicit Entity(EntityID id) 
		: m_id(id)
	{
	}

	std::function<void(Entity*)> destroyCallback{ nullptr };
	EventQueue events;

	void Update();
	void Render(sf::RenderTarget& target, sf::RenderStates& states);

	bool isAlive() const noexcept;
	void Destroy();
	void OnDestroy();

	bool isActive() const noexcept;
	void SetActive(bool val) noexcept;

	const std::string& GetName() const noexcept;
	void SetName(const std::string& name) noexcept;

	EntityID GetID() const noexcept;

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