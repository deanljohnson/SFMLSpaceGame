#pragma once
#include "Component.h"
#include "ParallaxMovement.h"

class ParallaxTargetAssigner : public Component
{
private:
	ParallaxMovement* m_parallax;
public:
	ParallaxTargetAssigner() 
		: m_parallax(nullptr)
	{}

	void Init() override;
	void Update() override;
};