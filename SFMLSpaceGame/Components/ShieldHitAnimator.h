#pragma once
#include "Shields.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <ExtendedMath.h>

class ShieldHitAnimator : public Component
{
private:
	// one center, 33 around the edge
	// with the number of edge points
	// being odd we get one vertex directly
	// in front
	const int NUM_VERTS = 34;
	const float VERT_ARC_LENGTH = M_TAU / (NUM_VERTS - 1);

	const char HIT_R = 0;
	const char HIT_G = 225;
	const char HIT_B = 225;
	const char HIT_A = 255;

	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };

	Shields* m_shields{ nullptr };
	sf::VertexArray m_vertextArray;
	sf::Transform m_transform;

	void OnShieldHit(Shields::Direction dir, const b2Vec2& dif);
public:
	explicit ShieldHitAnimator(float radius);

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};