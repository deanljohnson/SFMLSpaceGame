#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <cereal/access.hpp>

class Rotation : public Component
{
private:
	float m_radians;
	float m_degrees;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_radians);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Rotation>& construct)
	{
		EntityID selfID;
		float rad;
		ar(selfID, rad);
		construct(selfID, rad);
	}

public:
	explicit Rotation(EntityID ent, float radians = 0.f)
		: Component(ent), 
		  m_radians(radians), 
		  m_degrees(RAD_TO_DEG(radians))
	{}

	float GetDegrees() const { return m_degrees; }
	float GetRadians() const { return m_radians; }
	b2Vec2 GetHeading() const { return b2Vec2(cos(m_radians), sin(m_radians)); }

	void SetDegrees(float d) 
	{
		m_degrees = d;
		m_radians = DEG_TO_RAD(d);
	}

	void SetRadians(float r) 
	{
		m_radians = r;
		m_degrees = RAD_TO_DEG(r);
	}
};