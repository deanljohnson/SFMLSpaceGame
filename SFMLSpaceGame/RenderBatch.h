#pragma once
// based on https://github.com/SFML/SFML/wiki/Source:-High-Performance-Sprite-Container
#include <memory>
#include <unordered_map>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\VertexArray.hpp>

struct BatchIndex 
{
	explicit BatchIndex(unsigned i)
		: index(i) 
	{}
	unsigned index;
};

class RenderBatch 
{
private:
	static std::unordered_map<std::string, std::unique_ptr<RenderBatch>> m_stringBatches;

	std::vector<std::unique_ptr<BatchIndex>> m_indices;
	std::vector<BatchIndex*> m_removedIndices;

	std::shared_ptr<sf::Texture> m_texture;

	std::vector<sf::Vertex> m_vertices;
	std::vector<sf::IntRect> m_texRects;
	std::vector<sf::Vector2f> m_positions; // TODO: investigate removing this - m_vertices[index * 4].position gives the same information
	std::vector<sf::Vector2f> m_scales;
	std::vector<sf::Vector2f> m_origins;
	std::vector<b2Rot> m_rotations;

	void UpdateTexCoords(BatchIndex* index);
	void RemoveDeletedElements();
public:
	static RenderBatch* Get(const std::string& texName);
	static void RenderAll(sf::RenderTarget& target, sf::RenderStates states);

	explicit RenderBatch(std::shared_ptr<sf::Texture> tex);

	BatchIndex* Add();
	void Remove(BatchIndex* index);

	void Move(BatchIndex* index, float x, float y);
	void Move(BatchIndex* index, const sf::Vector2f& amt);

	void SetPosition(BatchIndex* index, const sf::Vector2f& pos);
	sf::Vector2f GetPosition(BatchIndex* index);

	void SetOrigin(BatchIndex* index, const sf::Vector2f& origin);
	sf::Vector2f GetOrigin(BatchIndex* index);
	sf::Vector2f GetScaledOrigin(BatchIndex* index);

	void SetRotation(BatchIndex* index, float rot);
	float GetRotation(BatchIndex* index);

	void SetScale(BatchIndex* index, const sf::Vector2f& scale);
	sf::Vector2f GetScale(BatchIndex* index);

	void SetTextureRect(BatchIndex* index, const sf::IntRect& rect);
	sf::IntRect GetTextureRect(BatchIndex* index);

	void SetColor(BatchIndex* index, const sf::Color& color);
	sf::Color GetColor(BatchIndex* index);

	sf::Texture* GetTexture();

	void Render(sf::RenderTarget& target, sf::RenderStates states);
};