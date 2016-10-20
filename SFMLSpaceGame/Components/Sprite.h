#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <SFML/Graphics/Sprite.hpp>
#include <ResourceLoader.h>
#include <RenderBatch.h>

class Sprite : public Component
{
private:
	RenderBatch* m_batch{ nullptr };
	int m_batchIndex = -1;

	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };

	sf::Sprite m_sprite;

	std::shared_ptr<sf::Texture> m_tex;

public:
	explicit Sprite(ResourceID id);
	explicit Sprite(const std::string& location);

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;

	// Returns the dimensions of this sprite in meters
	virtual sf::FloatRect GetDimensions() const;
	b2Vec2 GetOrigin() const;
};