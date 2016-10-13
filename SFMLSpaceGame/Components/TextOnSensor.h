#pragma once
#include "Component.h"
#include <UI/UI_ID.h>

class EntitySensor;

class TextOnSensor : public Component
{
private:
	std::string m_text;
	UI_ID m_textID{ UI_ID_NULL };
	bool m_display{ false };

public:
	explicit TextOnSensor(const std::string& text)
		: m_text(text)
	{}

	virtual void Init() override;
	virtual void Update() override;
};