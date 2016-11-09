#pragma once
#include "Component.h"
#include "ParallaxMovement.h"

class ParallaxTargetAssigner : public Component
{
private:
	ParallaxMovement& m_parallax;
public:
	explicit ParallaxTargetAssigner(EntityID ent);

	void Update() override;
};