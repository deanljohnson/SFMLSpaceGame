#pragma once
#include <DefaultSerializeable.h>

class Position;
class Rotation;

class SoundListener : public Component, public DefaultSerializeable<SoundListener>
{
private:
	Position& m_position;
	Rotation& m_rotation;
public:
	explicit SoundListener(EntityID ent);
	virtual void Update() override;
};