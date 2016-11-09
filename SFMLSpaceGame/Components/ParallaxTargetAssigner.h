#pragma once
#include "Component.h"
#include "ParallaxMovement.h"
#include <DefaultSerializeable.h>

class ParallaxTargetAssigner : public Component, public DefaultSerializeable<ParallaxTargetAssigner>
{
private:
	ParallaxMovement& m_parallax;
public:
	explicit ParallaxTargetAssigner(EntityID ent);

	void Update() override;
};