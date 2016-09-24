#pragma once
#include "Component.h"

class Position : Component
{
private:
	sf::Vector2f m_vec;
public:
	Position() : m_vec(sf::Vector2f(0, 0)){}

	void Set(float x, float y) noexcept { m_vec.x = x; m_vec.y = y; }

	void SetX(float x) noexcept { m_vec.x = x; }
	void SetY(float y) noexcept { m_vec.y = y; }

	float X() const noexcept { return m_vec.x; }
	float Y() const noexcept { return m_vec.y; }
};