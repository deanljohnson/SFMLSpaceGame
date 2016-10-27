#include "stdafx.h"
#include <UI/HardPointWidget.h>
#include <VectorMath.h>

HardPointWidget::HardPointWidget(const sf::Vector2f& position)
{
	m_posControl = sf::CircleShape(6.f);
	m_posControl.setPosition(position);
	m_posControl.setOrigin(m_posControl.getRadius(), m_posControl.getRadius());
	m_posControl.setFillColor(sf::Color::White);

	m_angleControl = sf::CircleShape(3.f);
	m_angleControl.setPosition(position + sf::Vector2f(ARM_LENGTH, 0.f));
	m_angleControl.setOrigin(m_angleControl.getRadius(), m_angleControl.getRadius());
	m_posControl.setFillColor(sf::Color::White);

	m_arm.setPrimitiveType(sf::LineStrip);
	m_arm.append(sf::Vertex(m_posControl.getPosition(), sf::Color::White));
	m_arm.append(sf::Vertex(m_angleControl.getPosition(), sf::Color::White));
}

void HardPointWidget::Select()
{
	m_posControl.setFillColor(sf::Color::Green);
	m_angleControl.setFillColor(sf::Color::Red);
}

void HardPointWidget::UnSelect()
{
	m_posControl.setFillColor(sf::Color::White);
	m_angleControl.setFillColor(sf::Color::White);
}

bool HardPointWidget::PositionControlContains(float x, float y)
{
	return m_posControl.getGlobalBounds().contains(x, y);
}

bool HardPointWidget::AngleControlContains(float x, float y)
{
	return m_angleControl.getGlobalBounds().contains(x, y);
}

void HardPointWidget::SetPositionControlLocation(float x, float y)
{
	auto dif = sf::Vector2f(x, y) - m_posControl.getPosition();
	m_posControl.setPosition(x, y);
	m_arm[0].position = sf::Vector2f(x, y);

	m_angleControl.move(dif);
	m_arm[1].position = m_angleControl.getPosition();
}

void HardPointWidget::SetAngleControlLocation(float x, float y)
{
	auto dif = Normalize(sf::Vector2f(x, y) - m_posControl.getPosition());
	dif *= ARM_LENGTH;
	m_angleControl.setPosition(m_posControl.getPosition() + dif);

	m_arm[1].position = m_angleControl.getPosition();
}

void HardPointWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_arm, states);

	target.draw(m_posControl, states);
	target.draw(m_angleControl, states);
}

float HardPointWidget::GetAngle() const
{
	auto dif = m_angleControl.getPosition() - m_posControl.getPosition();
	return atan2f(dif.y, dif.x);
}

void HardPointWidget::SetAngle(float angle)
{
	m_angleControl.setPosition(m_posControl.getPosition() + B2VecToSFMLVec(Rotate(b2Vec2(ARM_LENGTH, 0), angle)));
	m_arm[1].position = m_angleControl.getPosition();
}