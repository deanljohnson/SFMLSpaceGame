#pragma once
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\VertexArray.hpp>

class RenderBatch;

struct BatchIndex
{
private:
	RenderBatch& m_batch;
protected:
	friend class RenderBatch;
	size_t vertCount; // the number of verts this has in the batch
	size_t vertIndex; // the index of this's first vertex in the batch
	size_t index; // the index of this's pos/rot/etc in the batch
public:
	explicit BatchIndex(size_t index, size_t vertIndex, RenderBatch& batch);

	void Move(float x, float y);
	void Move(const sf::Vector2f& amt);

	void SetPosition(const sf::Vector2f& pos);
	sf::Vector2f GetPosition();

	void SetOrigin(const sf::Vector2f& origin);
	sf::Vector2f GetOrigin();
	sf::Vector2f GetScaledOrigin();

	void SetRotation(float rot);
	float GetRotation();

	void SetScale(const sf::Vector2f& scale);
	sf::Vector2f GetScale();

	void SetRect(const sf::IntRect& rect);
	sf::IntRect GetRect();

	void SetColor(const sf::Color& color);
	sf::Color GetColor();

	sf::Texture* GetTexture();

	void Remove();

	sf::Vertex& operator[](size_t index);
};