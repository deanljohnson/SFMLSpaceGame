#pragma once
#include <Box2D/Dynamics/b2WorldCallbacks.h>

class ContactFilter : public b2ContactFilter
{
public:
	virtual bool ShouldCollide(b2Fixture* a, b2Fixture* b) override;
};