#pragma once
#include "Component.h"
#include "Position.h"
#include "Rotation.h"

class SoundListener : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
public:
	virtual void Init() override;
	virtual void Update() override;
};