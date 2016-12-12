#pragma once
#include <Components\Component.h>
#include <SFGUI/ProgressBar.hpp>
#include <UI/UI.h>

template<typename TGunComponent>
class GunHeatUIDisplay : public Component, public DefaultSerializeable<GunHeatUIDisplay<TGunComponent>>, public Updateable
{
private:
	Gun& m_gunType;
	sfg::ProgressBar::Ptr m_progressBar;

public:
	explicit GunHeatUIDisplay(EntityID ent)
		: Component(ent),
		  m_gunType(entity->GetComponent<TGunComponent>())
	{
		static_assert(std::is_base_of<Gun, TGunComponent>::value,
			"TGunComponent must implement Gun interface");
		static_assert(std::is_base_of<Component, TGunComponent>::value,
			"TGunComponent must inherit from Component");

		m_progressBar = sfg::ProgressBar::Create();
		m_progressBar->SetRequisition({ 75.f, 25.f });
		UI::Singleton->Add(m_progressBar);
	}

	virtual void Update() override
	{
		m_progressBar->SetFraction(m_gunType.GetNormalizedHeat());
	};

	virtual void OnEnable() override
	{
		UI::Singleton->Add(m_progressBar);
	}

	virtual void OnDisable() override
	{
		UI::Singleton->Remove(m_progressBar);
	}
};