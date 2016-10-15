#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <SFML/Graphics/Sprite.hpp>
#include <ResourceLoader.h>

class Sprite : public Component
{
private:
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
};