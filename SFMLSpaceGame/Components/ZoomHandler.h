#pragma once
#include "Component.h"
#include <cereal/access.hpp>

class ZoomHandler : public Component
{
private:
	// Multiplier to affect the zoom speed
	const float ZOOM_SCALE = .1f;

	float m_zoomFactor{ 1.f };
	float m_lastZoom{ 1.f };

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_zoomFactor, m_lastZoom);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<ZoomHandler>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		ar(construct->m_zoomFactor,
			construct->m_lastZoom);
	}
public:
	explicit ZoomHandler(EntityID ent) : Component(ent){}
	virtual void Update() override;
};