#pragma once
#include <Components/Component.h>
#include <SFML/Graphics/Shape.hpp>
#include <Box2D/Dynamics/b2Body.h>

class Position;
class Rotation;

class Physics : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
	b2Body* m_body{ nullptr };
	b2BodyType m_bodyType;
	float m_linDamping;

public:
	Physics() 
		: m_bodyType(b2_dynamicBody), m_linDamping(0.f)
	{}

	explicit Physics(b2BodyType t, float linearDamping) 
		: m_bodyType(t), m_linDamping(linearDamping)
	{}
	~Physics();

	virtual void Init() override;
	virtual void Update() override;

	void SetPosition(const b2Vec2& v);
	b2Vec2 GetPosition() const;

	void SetVelocity(const b2Vec2& v);
	b2Vec2 GetVelocity() const;

	b2Vec2 GetHeading();

	float GetAngularVelocity() const;

	float GetRotationRadians();

	b2Body* GetBody() { return m_body; }

	void AddShape(const sf::Shape& s, float density = 1.f);
};