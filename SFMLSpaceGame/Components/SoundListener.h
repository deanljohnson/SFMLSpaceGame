#pragma once
#include <DefaultSerializeable.h>

class Position;
class Rotation;

class SoundListener : public Component, public DefaultSerializeable<SoundListener>, public Updateable
{
private:
	Position& m_position;
	Rotation& m_rotation;
public:
	explicit SoundListener(EntityID ent);
	virtual void Update() override;
};