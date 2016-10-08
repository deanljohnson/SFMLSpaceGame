#pragma once
#include "Health.h"

class DamageOnAttacked : public Component
{
private:
	Health* m_health{ nullptr };
public:
	virtual void Init() override;
	virtual void Update() override;
};