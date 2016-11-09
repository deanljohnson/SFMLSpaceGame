#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <cereal/access.hpp>

class CirclePrimitive : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;
	sf::CircleShape m_shape;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_shape.getRadius());
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<CirclePrimitive>& construct)
	{
		EntityID selfID;
		float radius;
		ar(selfID, radius);
		construct(selfID, radius);
	}

public:
	CirclePrimitive(EntityID ent, float rad);

	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};