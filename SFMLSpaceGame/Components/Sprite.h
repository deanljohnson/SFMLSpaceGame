#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <ResourceLoader.h>
#include <RenderBatch.h>

class Sprite : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };

	RenderBatch* m_batch{ nullptr };
	BatchIndex* m_batchIndex;

public:
	explicit Sprite(ResourceID id);
	explicit Sprite(const std::string& location);
	~Sprite();

	virtual void Init() override;
	virtual void Update() override;

	// Returns the dimensions of this sprite in meters
	virtual sf::FloatRect GetDimensions() const;
	b2Vec2 GetOrigin() const;
};