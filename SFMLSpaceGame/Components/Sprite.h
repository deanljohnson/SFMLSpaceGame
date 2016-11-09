#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <RenderBatch.h>

class Sprite : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;

	RenderBatch* m_batch{ nullptr };
	BatchIndex* m_batchIndex;
	std::string m_location;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_location);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Sprite>& construct)
	{
		EntityID selfID;
		std::string location;
		ar(selfID, location);
		construct(selfID, location);
	}
public:
	Sprite(EntityID ent, const std::string& location);
	~Sprite();

	virtual void Update() override;

	// Returns the dimensions of this sprite in meters
	virtual sf::FloatRect GetDimensions() const;
	b2Vec2 GetOrigin() const;
};