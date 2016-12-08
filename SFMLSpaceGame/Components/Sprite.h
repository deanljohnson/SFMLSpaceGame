#pragma once
#include <OriginOption.h>
#include <SpriteKey.h>
#include <Components/Component.h>

class Position;
class Rotation;
class RenderBatch;
struct BatchIndex;

class Sprite : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;

	BatchIndex* m_batchIndex;

	b2Vec2 m_offset;

	SpriteKey m_key;

	OriginOption m_originOption;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(GetEntityID(), m_key, m_originOption, m_offset);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Sprite>& construct)
	{
		EntityID selfID;
		SpriteKey key;
		OriginOption option;
		ar(selfID, key, option);
		construct(selfID, key, option);

		ar(construct->m_offset);
	}

	void InitializeBatchRender();
public:
	Sprite(EntityID ent, const SpriteKey& key, OriginOption origin = OriginOption::Center);
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