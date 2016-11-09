#pragma once
#include "Shields.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <ExtendedMath.h>

class ShieldHitAnimator : public Component
{
private:
	bool m_visible;

	const int NUM_VERTS = 20;
	const float VERT_ARC_LENGTH = M_TAU / (NUM_VERTS - 1);

	const char HIT_R = 0;
	const char HIT_G = (char)225;
	const char HIT_B = (char)225;
	const char HIT_A = (char)225;

	Position& m_position;
	Rotation& m_rotation;

	sf::VertexArray m_vertextArray;
	sf::Transform m_transform;

	void OnShieldHit(Shields::Direction dir, const b2Vec2& dif);
public:
	ShieldHitAnimator(EntityID ent, float radius);

	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};