#pragma once
#include "Component.h"
#include "Position.h"
#include "Rotation.h"

class SoundListener : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;
public:
	explicit SoundListener(EntityID ent);
	virtual void Update() override;
};