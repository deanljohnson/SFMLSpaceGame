#include <Steering.h>
#include <VectorMath.h>
#include <Box2D/Common/b2Math.h>

b2Vec2 Steering::Seek(const b2Vec2& current, const b2Vec2& target)
{
	b2Vec2 ret = target - current;
	ret.Normalize();
	return ret;
}

b2Vec2 Steering::Flee(const b2Vec2& current, const b2Vec2& target)
{
	return -Seek(current, target);
}

b2Vec2 Steering::Pursue(Physics* self, Physics* target, float maxSpeed)
{
	b2Vec2 dif = target->GetPosition() - self->GetPosition();
	float t = dif.Length() / maxSpeed;

	b2Vec2 future = target->GetPosition() + (target->GetVelocity() * t);

	b2Vec2 ret = future - self->GetPosition();
	ret.Normalize();
	return ret;
}

b2Vec2 Steering::Evade(Physics* self, Physics* target, float maxSpeed)
{
	return -Pursue(self, target, maxSpeed);
}

b2Vec2 Steering::Follow(Physics* self, Physics* target, float followDistance)
{
	b2Vec2 reverse = (target->GetVelocity() * -1);
	reverse.Normalize();
	reverse *= followDistance; // now 'reverse' represent where we want to move towards

	b2Vec2 seekTarget = target->GetPosition() + reverse;
	return Seek(self->GetPosition(), seekTarget);
}

b2Vec2 Steering::AlignOrientation(std::vector<Physics*> others)
{
	float totalRotation = 0.f;
	for (auto p : others)
	{
		totalRotation += p->GetRotationRadians();
	}
	float avgRotation = totalRotation / others.size();
	return Rotate(b2Vec2(1, 0), avgRotation);
}

b2Vec2 Steering::AlignHeading(std::vector<Physics*> others)
{
	b2Vec2 totalVelocity = b2Vec2_zero;
	for (auto p : others)
	{
		totalVelocity += p->GetVelocity();
	}
	b2Vec2 avgVelocity = totalVelocity / others.size();
	avgVelocity.Normalize();
	return avgVelocity;
}