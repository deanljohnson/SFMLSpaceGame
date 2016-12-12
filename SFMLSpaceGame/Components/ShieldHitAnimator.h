#pragma once
#include "Shields.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <ExtendedMath.h>
#include <DefaultSerializeable.h>
#include <Components/Interfaces/Renderable.h>

class ShieldHitAnimator : public Component, public DefaultSerializeable<ShieldHitAnimator>, public Updateable, public Renderable
{
private:
	bool m_visible;
	float m_radius;

	const size_t NUM_VERTS = 20;
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

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_radius);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<ShieldHitAnimator>& construct)
	{
		EntityID selfID;
		float radius;
		ar(selfID, radius);
		construct(selfID, radius);
	}
public:
	ShieldHitAnimator(EntityID ent, float radius);

	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};