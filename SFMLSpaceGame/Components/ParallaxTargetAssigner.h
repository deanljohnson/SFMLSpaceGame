#pragma once
#include <DefaultSerializeable.h>

class ParallaxMovement;

class ParallaxTargetAssigner : public Component, public DefaultSerializeable<ParallaxTargetAssigner>, public Updateable
{
private:
	ParallaxMovement& m_parallax;
public:
	explicit ParallaxTargetAssigner(EntityID ent);

	void Update() override;
};