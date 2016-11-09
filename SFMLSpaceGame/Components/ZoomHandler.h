#pragma once
#include "Component.h"

class ZoomHandler : public Component
{
private:
	// Multiplier to affect the zoom speed
	const float ZOOM_SCALE = .1f;

	float m_zoomFactor{ 1.f };
	float m_lastZoom{ 1.f };
public:
	explicit ZoomHandler(EntityID ent) : Component(ent){}
	virtual void Update() override;
};