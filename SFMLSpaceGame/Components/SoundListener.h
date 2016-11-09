#pragma once
#include "Component.h"
#include "Position.h"
#include "Rotation.h"
#include <DefaultSerializeable.h>

class SoundListener : public Component, public DefaultSerializeable<SoundListener>
{
private:
	Position& m_position;
	Rotation& m_rotation;
public:
	explicit SoundListener(EntityID ent);
	virtual void Update() override;
};