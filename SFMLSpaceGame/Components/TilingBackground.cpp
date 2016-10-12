#include <Components/TilingBackground.h>
#include <ResourceLoader.h>
#include <Entity.h>
#include <Components/Position.h>
#include <WorldConstants.h>
#include <VectorMath.h>

void TilingBackground::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_tex = LoadTextureResource(m_resourceID);
	m_sprite = sf::Sprite(*m_tex.get());
	m_sprite.scale(2.f * METERS_PER_PIXEL, 2.f * METERS_PER_PIXEL); //scale to be in terms of meters - the render function will scale back to pixels
}

void TilingBackground::Update()
{
	m_sprite.setPosition(B2VecToSFMLVec(m_position->position));
}

void TilingBackground::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	auto bounds = m_sprite.getGlobalBounds();
	bounds = states.transform.transformRect(bounds);

	// Coordinates of the corners of the window
	auto topLeftCoord = target.mapPixelToCoords(sf::Vector2i(0, 0));
	auto botRightCoord = target.mapPixelToCoords(sf::Vector2i(target.getSize().x, target.getSize().y));
	auto centerCoord = target.mapPixelToCoords(sf::Vector2i(target.getSize().x / 2, target.getSize().y / 2));

	sf::Vector2i min = sf::Vector2i(0, 0);
	sf::Vector2i max = sf::Vector2i(0, 0);

	// calc the min tile on screen
	auto topLeftDif = topLeftCoord - centerCoord;
	min.x = (int)(topLeftDif.x / bounds.width);
	min.y = (int)(topLeftDif.y / bounds.height);

	// calc the max tile on screen
	auto botRightDif = botRightCoord - centerCoord;
	max.x = (int)(botRightDif.x / bounds.width);
	max.y = (int)(botRightDif.y / bounds.height);

	// Increase the area drawn so we make sure to catch any
	// fractional tiles that need to be displayed
	min.x--;
	min.y--;
	max.x++;
	max.y++;

	// Move the min/max points to account for what
	// the current center tile is
	min.x += centerCoord.x / bounds.width;
	min.y += centerCoord.y / bounds.height;
	max.x += centerCoord.x / bounds.width;
	max.y += centerCoord.y / bounds.height;

	// Move the transform to the min tile position
	states.transform.translate(bounds.width * (min.x - 1), bounds.height * (min.y - 1));

	// Need an untransfored bounds to adjust by
	bounds = m_sprite.getGlobalBounds();
	for (int j = min.y; j <= max.y + 1; j++)
	{
		for (int i = min.x; i <= max.x + 1; i++)
		{
			target.draw(m_sprite, states);
			states.transform.translate(bounds.width, 0);
		}
		states.transform.translate(-bounds.width * (max.x - min.x + 2), bounds.height);
	}
}