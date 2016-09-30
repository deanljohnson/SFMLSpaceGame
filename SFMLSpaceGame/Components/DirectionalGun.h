#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>

class DirectionalGun : public Component 
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
public:
	virtual void Init() override;

	void Shoot() const;
};