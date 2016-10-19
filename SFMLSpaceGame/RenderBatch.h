#pragma once
// based on https://github.com/SFML/SFML/wiki/Source:-High-Performance-Sprite-Container
#include <memory>
#include <unordered_map>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\VertexArray.hpp>

class RenderBatch 
{
private:
	static std::unordered_map<std::string, RenderBatch> m_batches;

	std::vector<sf::Vertex> m_vertices;
	std::shared_ptr<sf::Texture> m_texture;

	std::vector<sf::IntRect> m_texRects;
	std::vector<sf::Vector2f> m_positions;
	std::vector<sf::Vector2f> m_scales;
	std::vector<float> m_angles;

	void UpdateTexCoords(const unsigned index);
	void UpdateVertexCoords(const unsigned index, bool reset);
public:
	RenderBatch(std::shared_ptr<sf::Texture> tex);

	void Move(const unsigned index, float x, float y);
	void Move(const unsigned index, const sf::Vector2f& amt);

	void SetPosition(const unsigned index, const sf::Vector2f& pos);
	sf::Vector2f GetPosition(const unsigned index);

	void SetRotation(const unsigned index, float rot);
	float GetRotation(const unsigned index);

	void SetScale(const unsigned index, const sf::Vector2f& scale);
	sf::Vector2f GetScale(const unsigned index);

	void SetTextureRect(const unsigned index, const sf::IntRect& rect);
	sf::IntRect GetTextureRext(const unsigned index);

	void Render(sf::RenderTarget& target, sf::RenderStates states);
};