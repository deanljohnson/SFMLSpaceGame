#pragma once
#include <Components/Component.h>

class Lifetime : public Component
{
private:
	float m_lifetime;
public:
	explicit Lifetime(float lifetime)
		: m_lifetime(lifetime)
	{}

	virtual void Update() override;
};
