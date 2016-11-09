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

public:
	Sprite(EntityID ent, const std::string& location);
	~Sprite();

	virtual void Update() override;

	// Returns the dimensions of this sprite in meters
	virtual sf::FloatRect GetDimensions() const;
	b2Vec2 GetOrigin() const;
};