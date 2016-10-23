#pragma once
#include "Component.h"
#include "Rotation.h"
#include "Position.h"
#include <ResourceLoader.h>
#include <Animation.h>
#include <OriginOption.h>
#include <RenderBatch.h>

class AnimatedSprite : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };

	RenderBatch* m_batch{ nullptr };
	BatchIndex* m_batchIndex;

	Animation m_animation;

	b2Vec2 m_offset;

public:
	explicit AnimatedSprite(ResourceID id, OriginOption origin = OriginOption::Center);
	explicit AnimatedSprite(const std::string& id, OriginOption origin = OriginOption::Center);
	~AnimatedSprite();

	virtual void Init() override;
	virtual void Update() override;

	void SetOffset(const b2Vec2& v);
	void SetScale(float x, float y);

	Animation* GetAnimation();

	// Returns the dimensions of this sprite in meters
	virtual sf::FloatRect GetDimensions() const;
};