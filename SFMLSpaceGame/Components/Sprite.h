#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <OriginOption.h>
#include <RenderBatch.h>

class Sprite : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;

	RenderBatch* m_batch{ nullptr };
	BatchIndex* m_batchIndex;

	b2Vec2 m_offset;

	std::string m_id;

	OriginOption m_originOption;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_id, m_originOption, m_offset);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Sprite>& construct)
	{
		EntityID selfID;
		std::string id;
		OriginOption option;
		ar(selfID, id, option);
		construct(selfID, id, option);

		ar(construct->m_offset);
	}
public:
	Sprite(EntityID ent, const std::string& id, OriginOption origin = OriginOption::Center);
	~Sprite();

	virtual void Update() override;

	void SetOffset(const b2Vec2& v);
	void SetScale(float x, float y);
	void SetTextureRect(const sf::IntRect& rect);

	// Returns the dimensions of this sprite in meters
	sf::FloatRect GetDimensions() const;
	b2Vec2 GetOrigin() const;
};