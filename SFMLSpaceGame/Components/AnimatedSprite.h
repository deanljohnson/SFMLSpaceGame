#pragma once
#include "Component.h"
#include "Rotation.h"
#include "Position.h"
#include <Animation.h>
#include <OriginOption.h>
#include <RenderBatch.h>
#include <cereal/access.hpp>
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

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_id, m_originOption, m_offset);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<AnimatedSprite>& construct)
	{
		EntityID selfID;
		std::string id;
		OriginOption option;
		ar(selfID, id, option);
		construct(selfID, id, option);

		ar(construct->m_offset);
	}

public:
	explicit AnimatedSprite(EntityID ent, const std::string& id, OriginOption origin = OriginOption::Center);
	~AnimatedSprite();

	virtual void Update() override;

	void SetOffset(const b2Vec2& v);
	void SetScale(float x, float y);
	void SetTextureRect(const sf::IntRect& rect);

	Animation* GetAnimation();

	// Returns the dimensions of this sprite in meters
	sf::FloatRect GetDimensions() const;
};