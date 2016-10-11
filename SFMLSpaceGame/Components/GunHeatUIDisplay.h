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
	virtual void Init() 
	{
		static_assert(std::is_base_of<Gun, TGunComponent>::value,
			"TGunComponent must implement Gun interface");
		static_assert(std::is_base_of<Component, TGunComponent>::value,
			"TGunComponent must inherit from Component");

		m_gunType = &entity->GetComponent<TGunComponent>();

		m_sliderID = UI::Init<UISlider, float, float, ResourceID>(11.f, 11.f, UI_SLIDER_ONE);
		//INIT_AND_DISPLAY(UISlider, m_sliderID, 11.f, 11.f, UI_SLIDER_ONE);
	}
	virtual void Update() 
	{
		m_sliderID = UI::Display<UISlider, float>(m_sliderID, m_gunType->GetNormalizedHeat());
	};
};