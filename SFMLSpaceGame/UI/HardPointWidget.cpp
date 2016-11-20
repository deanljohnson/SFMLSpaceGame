#include "stdafx.h"
#include <UI/HardPointWidget.h>
#include <VectorMath.h>

HardPointWidget::HardPointWidget(const sf::Vector2f& position, Type type)
	: m_posControl(),
	  m_angleControl(new sf::CircleShape(3.f)),
	  m_type(type)
{
	// this is only used for missile launcher types, 
	// but we cannoy define/initialize this inside a
	// switch statement because of the automatic
	// cleanup mechanics of a unique ptr
	std::unique_ptr<sf::ConvexShape> mlShape{ new sf::ConvexShape() };

	switch (m_type)
	{
	case Type::Gun:
		m_posControl = std::make_unique<sf::CircleShape>(6.f);
		break;
	case Type::MissileLauncher:
		mlShape->setPointCount(3);
		mlShape->setPoint(0, { 0.f, 0.f });
		mlShape->setPoint(1, { 12.f, 6.f });
		mlShape->setPoint(2, { 0.f, 12.f });
		m_posControl = std::move(mlShape);
		break;
	default:
		throw "unrecognized hard point type";
	}

	m_posControl->setPosition(position);
	m_posControl->setOrigin(6.f, 6.f);
	m_posControl->setFillColor(sf::Color::White);

	m_angleControl->setPosition(position + sf::Vector2f(ARM_LENGTH, 0.f));
	m_angleControl->setOrigin(3.f, 3.f);
	m_posControl->setFillColor(sf::Color::White);

	m_arm.setPrimitiveType(sf::LineStrip);
	m_arm.append(sf::Vertex(m_posControl->getPosition(), sf::Color::White));
	m_arm.append(sf::Vertex(m_angleControl->getPosition(), sf::Color::White));
}

void HardPointWidget::Select()
{
	m_posControl->setFillColor(sf::Color::Green);
	m_angleControl->setFillColor(sf::Color::Red);
}

void HardPointWidget::UnSelect()
{
	m_posControl->setFillColor(sf::Color::White);
	m_angleControl->setFillColor(sf::Color::White);
}

bool HardPointWidget::PositionControlContains(float x, float y)
{
	return m_posControl->getGlobalBounds().contains(x, y);
}

bool HardPointWidget::AngleControlContains(float x, float y)
{
	return m_angleControl->getGlobalBounds().contains(x, y);
}

void HardPointWidget::SetPositionControlLocation(float x, float y)
{
	auto dif = sf::Vector2f(x, y) - m_posControl->getPosition();
	m_posControl->setPosition(x, y);
	m_arm[0].position = sf::Vector2f(x, y);

	m_angleControl->move(dif);
	m_arm[1].position = m_angleControl->getPosition();
}

void HardPointWidget::SetAngleControlLocation(float x, float y)
{
	auto dif = sf::Vector2f(x, y) - m_posControl->getPosition();
	SetAngle(atan2f(dif.y, dif.x));
}

sf::Vector2f HardPointWidget::GetHardPointLocation() const
{
	return m_posControl->getPosition();
}

void HardPointWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_arm, states);

	target.draw(*m_posControl, states);
	target.draw(*m_angleControl, states);
}

float HardPointWidget::GetAngle() const
{
	auto dif = m_angleControl->getPosition() - m_posControl->getPosition();
	return atan2f(dif.y, dif.x);
}

void HardPointWidget::SetAngle(float angle)
{
	m_angleControl->setPosition(m_posControl->getPosition() + B2VecToSFMLVec(Rotate(b2Vec2(ARM_LENGTH, 0), angle)));
	m_arm[1].position = m_angleControl->getPosition();

	m_posControl->setRotation(RAD_TO_DEG(angle));
}

HardPointWidget::Type HardPointWidget::GetType() const
{
	return m_type;
}