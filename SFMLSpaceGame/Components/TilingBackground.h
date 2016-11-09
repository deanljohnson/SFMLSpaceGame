#pragma once
#include "Component.h"
#include <ResourceLoader.h>
#include <SFML/Graphics/Sprite.hpp>

class Position;

class TilingBackground : public Component
{
private:
	Position& m_position;
	std::shared_ptr<sf::Texture> m_tex;
	sf::Sprite m_sprite;
	ResourceID m_resourceID;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_resourceID);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<TilingBackground>& construct)
	{
		EntityID selfID;
		ResourceID resourceID;
		ar(selfID, resourceID);
		construct(selfID, resourceID);
	}
public:
	TilingBackground(EntityID ent, ResourceID resourceID);

	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};