#pragma once
#include <Components\Component.h>

template<typename TGunComponent>
class GunHeatUIDisplay : public Component 
{
private:
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
	};
};