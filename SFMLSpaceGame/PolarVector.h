#pragma once
// used from https://github.com/SFML/SFML/wiki/Source:-High-Performance-Sprite-Container
#include <SFML\System\Vector2.hpp>

class PolarVector
{
public:
	float r;
	float t;

	PolarVector();
	PolarVector(float radius, float angle);

	sf::Vector2f TurnToRectangular() const;
private:
	static const float EPSILON;

	friend bool operator ==(const PolarVector& left, const PolarVector& right);
};

PolarVector TurnToPolar(const sf::Vector2f& point);

bool operator ==(const PolarVector& left, const PolarVector& right);

bool operator !=(const PolarVector& left, const PolarVector& right);
