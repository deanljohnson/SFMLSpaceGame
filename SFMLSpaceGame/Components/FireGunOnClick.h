#pragma once
#include <Components/Component.h>
#include <Interfaces/MouseListener.h>
#include <Interfaces/Gun.h>
#include <vector>
#include <DefaultSerializeable.h>

class FireGunOnClick : public Component, MouseListener, public DefaultSerializeable<FireGunOnClick>
{
private:
	std::vector<Gun*> m_guns;
public:
	explicit FireGunOnClick(EntityID ent);

	virtual void OnClick(b2Vec2 pos) override;
	virtual void OnHeld(b2Vec2 pos) override;
};