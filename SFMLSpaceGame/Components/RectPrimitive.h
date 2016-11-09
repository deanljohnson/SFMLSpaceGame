#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <cereal/access.hpp>

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
		ar(entity.GetID(), m_shape.getSize());
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<RectPrimitive>& construct)
	{
		EntityID selfID;
		sf::Vector2f size;
		ar(selfID, size);
		construct(selfID, size.x, size.y);
	}

public:
	RectPrimitive(EntityID ent, float w, float h);

	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
	
	const sf::RectangleShape& GetShape() const { return m_shape; }
};