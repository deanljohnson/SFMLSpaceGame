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
	virtual void Init() override;
	virtual void Update() override;
};