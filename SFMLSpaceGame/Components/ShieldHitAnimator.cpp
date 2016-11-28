#include "stdafx.h"
#include <Components/ShieldHitAnimator.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Entity.h>
#include <VectorMath.h>
#include <GameTime.h>

ShieldHitAnimator::ShieldHitAnimator(EntityID ent, float radius)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>())
{
	m_radius = radius;
	m_vertextArray.setPrimitiveType(sf::PrimitiveType::TriangleFan);

	// Center at 0,0
	m_vertextArray.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Transparent));

	float curAngle = -static_cast<float>(M_PI);
	// now loop through and do all the other vertices
	// negative side first
	for (size_t i = 0; i < NUM_VERTS; i++)
	{
		m_vertextArray.append(sf::Vertex(AtAngleRad(curAngle) * radius, sf::Color::Transparent));
		curAngle += VERT_ARC_LENGTH;
	}

	auto& m_shields = entity->GetComponent<Shields>();
	m_shields.SetShieldHitCallback(
		[this](Shields::Direction dir, const b2Vec2& dif)
	{
		OnShieldHit(dir, dif);
	});
}

void ShieldHitAnimator::Update()
{
	m_transform = sf::Transform();
	m_transform.translate(m_position.X(), m_position.Y())
		.rotate(m_rotation.GetDegrees());

	m_visible = false;
	for (int i = 0; i < m_vertextArray.getVertexCount(); i++)
	{
		if (m_vertextArray[i].color.a > 0)
		{
			m_visible = true;
			m_vertextArray[i].color.a = static_cast<char>(std::max(0.f, Lerp(m_vertextArray[i].color.a, 0.f, GameTime::deltaTime * 5)));
		}
	}
}

void ShieldHitAnimator::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	if (!m_visible)
		return;

	states.transform.combine(m_transform);

	target.draw(m_vertextArray, states);
}

void ShieldHitAnimator::OnShieldHit(Shields::Direction dir, const b2Vec2& dif)
{
	auto angle = atan2f(dif.y, dif.x);

	float testAngle = -static_cast<float>(M_PI);
	int index = 1;
	while(testAngle < angle)
	{
		index++;
		testAngle += VERT_ARC_LENGTH;
	}

	m_vertextArray[(index - 2) % (m_vertextArray.getVertexCount())].color = sf::Color(HIT_R, HIT_G, HIT_B, static_cast<char>(HIT_A * .75f));
	m_vertextArray[(index - 1) % (m_vertextArray.getVertexCount())].color = sf::Color(HIT_R, HIT_G, HIT_B, static_cast<char>(HIT_A * .75f));
	m_vertextArray[index].color = sf::Color(HIT_R, HIT_G, HIT_B, HIT_A);
	m_vertextArray[(index + 1) % (m_vertextArray.getVertexCount())].color = sf::Color(HIT_R, HIT_G, HIT_B, static_cast<char>(HIT_A * .75f));
	m_vertextArray[(index + 2) % (m_vertextArray.getVertexCount())].color = sf::Color(HIT_R, HIT_G, HIT_B, static_cast<char>(HIT_A * .75f));
}