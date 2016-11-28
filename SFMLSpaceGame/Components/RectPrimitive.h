#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <cereal/access.hpp>
#include <SFMLSerialization.h>

class Position;
class Rotation;

class RectPrimitive : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;
	sf::RectangleShape m_shape;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		sf::Vector2f size = m_shape.getSize();
		ar(entity.GetID(), size);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<RectPrimitive>& construct)
	{
		EntityID selfID;
		sf::Vector2f size;
		ar(selfID, size);
		construct(selfID, size);
	}

public:
	RectPrimitive(EntityID ent, float w, float h);
	RectPrimitive(EntityID ent, const sf::Vector2f& size);

	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
	
	const sf::RectangleShape& GetShape() const { return m_shape; }
};