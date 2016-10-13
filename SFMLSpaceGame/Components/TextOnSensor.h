#pragma once
#include "Component.h"
#include <UI/UI_ID.h>

class EntitySensor;

class TextOnSensor : public Component
{
private:
	EntitySensor* m_sensor{ nullptr };
	std::string m_text;
	UI_ID m_textID{ UI_ID_NULL };

public:
	explicit TextOnSensor(const std::string& text)
		: m_text(text)
	{}

	virtual void Init() override;
	virtual void Update() override;
};