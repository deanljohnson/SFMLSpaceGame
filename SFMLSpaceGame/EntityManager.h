#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <array>
#include <EntityHandle.h>
#include <Group.h>

namespace sf{
	class RenderTarget;
	class RenderStates;
}

class b2World;
class Entity;

class EntityManager
{
private:
	static EntityID m_nextID;
	static std::vector<std::unique_ptr<Entity>> m_entities;
	static std::unordered_map<EntityID, std::unique_ptr<EntityHandle>> m_entityHandles;
	static std::unordered_map<std::string, EntityID> m_namedEntities;
	static std::array<std::vector<Entity*>, maxGroups> m_groupedEntities;

	friend class Entity;
	static void OnEntityNameChange(EntityID id, const std::string& name, const std::string& oldName = "");

	friend class cereal::access;

	template<typename Archive>
	void save(Archive& ar) const
	{
		ar(m_entities);
	}

	template<typename Archive>
	void load(Archive& ar)
	{
		std::vector<std::unique_ptr<Entity>> entities;
		ar(entities);

		// Entities are responsible for registering themselves
		// with the entity manager. Just realase the pointers
		// from here so they don't get deleted
		for(auto& e : entities)
		{
			e.release();
		}
	}

public:
	static void Clear();

	static void Refresh();
	static void Update();
	static void Render(sf::RenderTarget& target, sf::RenderStates& states);

	static bool IsValidID(EntityID id);
	static EntityHandle Get(EntityID id);
	static EntityHandle GetByName(const std::string& name);

	static EntityHandle AddEntity();
	static EntityHandle AddEntity(Entity* ent);
	static EntityHandle AddEntity(Group group);

	static void AddToGroup(Entity* ent, Group group);
	static const std::vector<Entity*>& GetEntitiesByGroup(Group group);

	static std::string GetTypeName();
};