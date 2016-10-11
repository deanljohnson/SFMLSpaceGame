#pragma once
#include <Components\Component.h>
#include <UI/UI.h>
#include <UI/UISlider.h>

template<typename TGunComponent>
class GunHeatUIDisplay : public Component 
{
private:
	UI_ID m_sliderID;
	TGunComponent* m_gunType{ nullptr };

public:
	virtual void Init() override
	{
		static_assert(std::is_base_of<Gun, TGunComponent>::value,
			"TGunComponent must implement Gun interface");
		static_assert(std::is_base_of<Component, TGunComponent>::value,
			"TGunComponent must inherit from Component");

		m_gunType = &entity->GetComponent<TGunComponent>();
	}
	virtual void Update() override
	{
		INIT_AND_UPDATE(UISlider, m_sliderID,
			ON_INIT(11.f, 11.f, UI_SLIDER_ONE, sf::Color::Red),
			ON_UPDATE(m_gunType->GetNormalizedHeat()));
	};
};