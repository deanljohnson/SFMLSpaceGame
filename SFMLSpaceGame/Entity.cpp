#include "Entity.h"
#include "EntityManager.h"

void Entity::Update()
{
	for (auto& c : m_components) 
		c->Update();
}

void Entity::Render(sf::RenderTarget& target)
{
	for (auto& c : m_components) 
		c->Render(target);
}

template<typename T, typename... TArgs>
T& Entity::AddComponent(TArgs&&... args)
{
	//we only allow entities to have a single component of a type
	assert(!HasComponent<T>());

	//Call the components constructor with the given args
	T* c(new T(std::forward<TArgs>(args)...));

	//Set the owner to this
	c->entity = this;

	//wrap the raw pointer
	std::unique_ptr<Component> uPtr(c);

	//add to our set of components
	m_components.emplace_back(move(uPtr));

	m_componentArray[GetComponentTypeID<T>()] = c;
	m_componentBitset[GetComponentTypeID<T>()] = true;

	c->Init();

	//return in case caller needs this
	return *c;
}

template<typename T>
bool Entity::HasComponent() const
{
	return m_componentBitset[GetComponentTypeID<T>()];
}

/*template<typename T>
T& Entity::GetComponent() const
{
	//assert(HasComponent<T>());
	auto ptr{ m_componentArray[GetComponentTypeID<T>()] };
	return *static_cast<T*>(ptr);
}*/

bool Entity::HasGroup(Group group) const noexcept
{
	return m_groupBitset[group];
}

void Entity::DeleteFromGroup(Group group) noexcept
{
	m_groupBitset[group] = false;
}

void Entity::AddToGroup(Group group) noexcept 
{
	m_groupBitset[group] = true;
	m_manager.AddToGroup(this, group);
}