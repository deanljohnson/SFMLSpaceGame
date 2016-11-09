#pragma once
#include <Components/Component.h>

class Lifetime : public Component
{
private:
	float m_lifetime;
public:
	Lifetime(EntityID ent, float lifetime);

	virtual void Update() override;
};
