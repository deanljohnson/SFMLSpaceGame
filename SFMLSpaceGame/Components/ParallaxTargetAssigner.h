#pragma once
#include "Component.h"
#include "ParallaxMovement.h"

class ParallaxTargetAssigner : public Component
{
private:
	ParallaxMovement* m_parallax{ nullptr };
public:
	void Init() override;
	void Update() override;
};