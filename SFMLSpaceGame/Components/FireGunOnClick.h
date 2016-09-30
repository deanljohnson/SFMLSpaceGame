#pragma once
#include <Components/Component.h>
#include <Interfaces/MouseListener.h>
#include <Interfaces/Gun.h>
#include <vector>

class FireGunOnClick : public Component, MouseListener
{
private:
	std::vector<Gun*> m_guns;
public:
	virtual void Init() override;

	virtual void OnClick(b2Vec2 pos) override;
	virtual void OnHeld(b2Vec2 pos) override;
};