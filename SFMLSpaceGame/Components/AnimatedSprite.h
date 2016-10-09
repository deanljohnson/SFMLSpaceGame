#pragma once
#include "Component.h"
#include <SFML/Graphics/Sprite.hpp>
#include "Rotation.h"
#include "Position.h"
#include <ResourceLoader.h>
#include <Animation.h>
#include <OriginOption.h>

class AnimatedSprite : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };

	sf::Sprite m_sprite;
	OriginOption m_origin;

	ResourceID m_resourceID;
	std::shared_ptr<Animation> m_animation;

	b2Vec2 m_offset;

public:
	explicit AnimatedSprite(ResourceID id, OriginOption origin = OriginOption::Center)
		: m_origin(origin), m_resourceID(id)
	{}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates& states) override;

	void SetOffset(const b2Vec2& v);
	void SetScale(float x, float y);

	// Returns the dimensions of this sprite in meters
	virtual sf::FloatRect GetDimensions() const;
};