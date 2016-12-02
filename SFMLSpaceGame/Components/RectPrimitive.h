#pragma once
#include <cereal/access.hpp>
#include <SFMLSerialization.h>

class Position;
class Rotation;
class RenderBatch;
struct BatchIndex;

class RectPrimitive : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;
	sf::Vector2f m_size;

	RenderBatch* m_batch{ nullptr };
	BatchIndex* m_batchIndex;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_size);
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
	~RectPrimitive();

	virtual void Update() override;
};