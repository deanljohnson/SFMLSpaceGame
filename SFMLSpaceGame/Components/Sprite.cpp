#include <Components/Sprite.h>
#include <Entity.h>
#include <WorldConstants.h>
#include <VectorMath.h>

Sprite::Sprite(ResourceID id)
	: m_tex(LoadTexture(id))
{
}

Sprite::Sprite(const std::string& location)
{
	m_batch = RenderBatch::Get(location);
	m_batchIndex = m_batch->Add();
}

void Sprite::Init()
{
	if (m_batchIndex != -1)
	{
		m_position = &entity->GetComponent<Position>();
		m_rotation = &entity->GetComponent<Rotation>();

		auto rect = m_batch->GetTextureRect(m_batchIndex);

		m_batch->SetScale(m_batchIndex, sf::Vector2f(METERS_PER_PIXEL, METERS_PER_PIXEL));
		m_batch->SetOrigin(m_batchIndex, sf::Vector2f(rect.width / 2.f, rect.height / 2.f));
		m_batch->SetPosition(m_batchIndex, B2VecToSFMLVec(m_position->position));
		m_batch->SetRotation(m_batchIndex, m_rotation->GetDegrees());

		return;
	}

	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();

	m_sprite = sf::Sprite(*m_tex.get());
	m_sprite.setScale(METERS_PER_PIXEL, METERS_PER_PIXEL); //Render method will convert meters back to pixels

	m_sprite.setOrigin(sf::Vector2f(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f));
	m_sprite.setPosition(B2VecToSFMLVec(m_position->position));
	m_sprite.setRotation(m_rotation->GetDegrees());
}

void Sprite::Update()
{
	if (m_batchIndex != -1) 
	{
		m_batch->SetPosition(m_batchIndex, B2VecToSFMLVec(m_position->position));
		m_batch->SetRotation(m_batchIndex, m_rotation->GetDegrees());

		return;
	}

	m_sprite.setPosition(B2VecToSFMLVec(m_position->position));
	m_sprite.setRotation(m_rotation->GetDegrees());
}

void Sprite::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	if (m_batchIndex != -1)
		return;

	target.draw(m_sprite, states);
}

sf::FloatRect Sprite::GetDimensions() const
{
	if (m_batchIndex != -1) 
	{
		auto rect = m_batch->GetTextureRect(m_batchIndex);
		auto scale = m_batch->GetScale(m_batchIndex);

		return sf::FloatRect(rect.left, rect.top, rect.width * scale.x, rect.height * scale.y);
	}

	auto bounds = m_sprite.getLocalBounds();
	bounds.width *= m_sprite.getScale().x;
	bounds.height *= m_sprite.getScale().y;

	return sf::FloatRect(bounds.left, bounds.top, bounds.width, bounds.height);
}

b2Vec2 Sprite::GetOrigin() const
{
	if (m_batchIndex != -1)
		return SFMLVecToB2Vec(m_batch->GetOrigin(m_batchIndex));

	return SFMLVecToB2Vec(m_sprite.getOrigin() * METERS_PER_PIXEL);
}