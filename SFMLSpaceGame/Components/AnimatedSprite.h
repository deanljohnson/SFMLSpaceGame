#pragma once
#include "Component.h"
#include "Rotation.h"
#include "Position.h"
#include <ResourceLoader.h>
#include <Animation.h>
#include <OriginOption.h>
#include <RenderBatch.h>
#include <cereal/cereal.hpp>
#include <EntityID.h>

class AnimatedSprite : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;

	RenderBatch* m_batch{ nullptr };
	BatchIndex* m_batchIndex;

	Animation m_animation;

	b2Vec2 m_offset;

	OriginOption m_originOption;
	std::string m_id;

public:
	explicit AnimatedSprite(EntityID ent, const std::string& id, OriginOption origin = OriginOption::Center);
	~AnimatedSprite();

	virtual void Update() override;

	void SetOffset(const b2Vec2& v);
	void SetScale(float x, float y);

	Animation* GetAnimation();

	// Returns the dimensions of this sprite in meters
	virtual sf::FloatRect GetDimensions() const;

	// used for saving
	template <class Archive>
	void serialize(Archive & ar)
	{
		ar(m_id, m_originOption);
	}

	template <class Archive>
	static void load_and_construct( Archive & ar, cereal::construct<AnimatedSprite> & construct )
	{
		std::string id;
		OriginOption option;
		ar(id, option);
		construct(id, option);
	}
};